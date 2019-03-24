require("@babel/polyfill");
require("js-cookie");

const $ = require('jquery');
window.jQuery = $;
window.$ = $;

var context = require.context('./test/client', true, /gerbera.app.spec.js$/);
context.keys().forEach(context);