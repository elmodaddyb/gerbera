/*GRB*

    Gerbera - https://gerbera.io/

    gerbera-auth.module.js - this file is part of Gerbera.

    Copyright (C) 2016-2019 Gerbera Contributors

    Gerbera is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    Gerbera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gerbera.  If not, see <http://www.gnu.org/licenses/>.

    $Id$
*/
import {GerberaApp} from './gerbera-app.module';
import {Tree} from './gerbera-tree.module';
import {Items} from './gerbera-items.module';
import {Menu} from './gerbera-menu.module';
import {Trail} from './gerbera-trail.module';
import {Autoscan} from './gerbera-autoscan.module';
import {Updates} from './gerbera-updates.module';
import Cookie from '../vendor/js-cookie';

const checkSID = () => {
  return $.ajax({
    url: GerberaApp.clientConfig.api,
    type: 'get',
    async: false,
    data: {
      req_type: 'auth',
      sid: getSessionId(),
      action: 'get_sid'
    }
  }).then((response) => {
    return loadSession(response);
  }).catch((err) => {
    GerberaApp.error(err);
  });
};

const logout = () => {
  return $.ajax({
    url: GerberaApp.clientConfig.api,
    type: 'get',
    async: false,
    data: {
      req_type: 'auth',
      sid: getSessionId(),
      action: 'logout'
    }
  }).then(() => {
    handleLogout();
  }).catch((err) => {
    GerberaApp.error(err);
  });
};

const authenticate = () => {
  const username = $('#username').val();
  const password = $('#password').val();
  let promise;

  if (username.length > 0 && password.length > 0) {
    $('#warning').hide();
    promise =
      $.ajax({
        url: GerberaApp.clientConfig.api,
        type: 'get',
        data: {
          req_type: 'auth',
          sid: getSessionId(),
          action: 'get_token'
        }
      }).then((response) => {
        return submitLogin(response);
      }).catch((err) => {
        GerberaApp.error(err);
      });
  } else {
    Updates.showMessage('Please enter username and password', undefined, 'warning', 'fa-sign-in')
    promise = new Promise();
  }
  return promise
};

const submitLogin = (response) => {
  if (response.success) {
    const username = $('#username').val();
    let password = $('#password').val();
    const token = response.token;
    password = md5(token + password);
    return $.ajax({
      url: GerberaApp.clientConfig.api,
      type: 'get',
      data: {
        req_type: 'auth',
        sid: getSessionId(),
        action: 'login',
        username: username,
        password: password
      }
    }).then((response) => {
      checkLogin(response);
    }).catch((err) => {
      GerberaApp.error(err);
    });
  } else {
    GERBERA.Updates.showMessage(response.error.text, undefined, 'warning', 'fa-exclamation-circle')
  }
};

const checkLogin = (response) => {
  if (response.success) {
    $('.login-field').hide();
    $('#login-submit').hide();
    if (GerberaApp.serverConfig.accounts) {
      $('#logout').show().click(logout);
    }
    GerberaApp.setLoggedIn(true);
    Tree.initialize();
    Items.initialize();
    Trail.initialize();
    Menu.initialize();
    Autoscan.initialize();
    Updates.initialize();
  }
};

const getSessionId = () => {
  return Cookie.get('SID');
};

const handleLogout = () => {
  const now = new Date();
  const expire = new Date();
  GerberaApp.setLoggedIn(false);
  expire.setTime(now.getTime() - 3600000 * 24 * 360);
  Cookie.set('SID', null, {expires: expire});
  GerberaApp.reload('/index.html');
};

const loadSession = (response) => {
  return new Promise((resolve, reject) => {
    if (!response.sid_was_valid && response.sid && response.sid !== null) {
      Cookie.set('SID', response.sid);
    }
    // TODO: reject what?
    resolve(response.logged_in);
  });
};

export const Auth = {
  authenticate,
  checkSID,
  getSessionId,
  handleLogout,
  logout,
};