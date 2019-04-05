require("@babel/polyfill");
require("js-cookie");

const $ = require('jquery');
window.jQuery = $;
window.$ = $;

require('jquery-ui');
require('popper.js');
require('bootstrap');
require('../web/js/jquery.gerbera.autoscan.js');
require('../web/js/jquery.gerbera.items.js');
require('../web/js/jquery.gerbera.editor.js');
require('../web/js/jquery.gerbera.trail.js');
require('../web/js/jquery.gerbera.tree.js');

var context = require.context('./test/client', true, /gerbera.(app|auth|menu|updates).spec.js$/);
context.keys().forEach(context);

require('./test/client/gerbera.autoscan.spec');
require('./test/client/gerbera.items.spec');
require('./test/client/gerbera.trail.spec');
require('./test/client/gerbera.tree.spec');