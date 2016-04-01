'use strict';

if (process.platform !== 'linux') return;

var mouseForwardBack = require('bindings')('mouse-forward-back');

module.exports = {
    register: function(callback, handle) {
        mouseForwardBack(function(msg) {
          if (msg === 8) {
            callback("back");
          } else if (msg === 9) {
            callback("forward");
          }
        }, handle);
    }
}
