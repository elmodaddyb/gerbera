require("@babel/polyfill");
require("js-cookie");

const $ = require('jquery');
window.jQuery = $;
window.$ = $;

require('jquery-ui');
require('popper.js');
require('bootstrap');
require('../web/js/jquery.gerbera.autoscan.js');

var context = require.context('./test/client', true, /gerbera.(app|auth).spec.js$/);
context.keys().forEach(context);

require('./test/client/gerbera.autoscan.spec');