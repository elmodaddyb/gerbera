
import {Auth} from './gerbera-auth.module';
import {Autoscan} from './gerbera-autoscan.module';
import Cookie from '../vendor/js-cookie';
import {Items} from './gerbera-items.module';
import {Menu} from './gerbera-menu.module';
import {Trail} from './gerbera-trail.module';
import {Tree} from './gerbera-tree.module';
import {Updates} from './gerbera-updates.module';

export class App {

  constructor(clientConfig, serverConfig) {
    this.clientConfig = clientConfig;
    this.serverConfig = serverConfig;
  }

  isTypeDb() {
    return this.getType() === 'db'
  }

  getType() {
    return Cookie.get('TYPE');
  }

  setType(type) {
    Cookie.set('TYPE', type);
  }

  initialize () {
    return Updates.initialize()
      .then(() => this.configureDefaults())
      .then(() => {
        return this.getConfig(this.clientConfig);
      })
      .then((response) => {
        return this.loadConfig(response)
      })
      .then((serverConfig) => {
        this.serverConfig = serverConfig;
        return Auth.checkSID();
      })
      .then(() => {
        this.displayLogin();
        Menu.initialize(this.serverConfig);
      })
      .catch((error) => {
        this.error(error);
      });
  }

  configureDefaults () {
    if (this.getType() === undefined) {
      this.setType('db');
    }
    $.ajaxSetup({
      beforeSend: function (xhr) {
        xhr.setRequestHeader('Cache-Control', 'no-cache, must-revalidate');
      }
    });
    return $.Deferred().resolve().promise()
  }

  getConfig (clientConfig) {
    return $.ajax({
      url: clientConfig.api,
      type: 'get',
      async: false,
      data: {
        req_type: 'auth',
        sid: Auth.getSessionId(),
        action: 'get_config'
      }
    });
  }

  loadConfig (response) {
    return new Promise((resolve, reject) => {
      let serverConfig;
      if (response.success) {
        serverConfig = $.extend({}, response.config);
        var pollingInterval = response.config['poll-interval'];
        serverConfig['poll-interval'] = parseInt(pollingInterval) * 1000;
        if(serverConfig.friendlyName && serverConfig.friendlyName !== "Gerbera") {
          $(document).attr('title', serverConfig.friendlyName  + " | Gerbera Media Server");
        }
        resolve(serverConfig);
      } else {
        reject(response);
      }
    });
  }

  displayLogin () {
    if (Auth.isLoggedIn()) {
      $('.login-field').hide();
      $('#login-submit').hide();
      if (this.accounts) {
        $('#logout').show().click(Auth.logout)
      }
      Items.initialize();
      Tree.initialize();
      Trail.initialize();
      Autoscan.initialize();
      Updates.initialize();
    } else {
      $('.login-field').show();
      $('#login-form').submit(function (event) {
        Auth.authenticate(event);
        event.preventDefault();
      });
      $('#logout').hide();
    }
    return Promise.resolve();
  }

  error (event) {
    let message, type, icon;
    if (typeof event === 'string') {
      message = event;
      type = 'warning';
      icon = 'fa-exclamation-triangle';
    } else if (event && event.responseText) {
      message = event.responseText;
      type = 'info';
      icon = 'fa-exclamation-triangle';
    } else if (event && event.error) {
      message = event.error.text;
      type = 'warning';
      icon = 'fa-exclamation-triangle';
    } else {
      message = 'The system encountered an error';
      type = 'danger';
      icon = 'fa-frown-o';
    }
    Updates.showMessage(message, undefined, type, icon)
  }

  disable() {
    Menu.disable();
    Menu.hideLogin();
    Menu.hideMenu();
    Tree.destroy();
    Trail.destroy();
    Items.destroy();
  }
}

export let GerberaApp = new App({
  api: 'content/interface'
}, {});