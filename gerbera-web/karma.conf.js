module.exports = function(config) {
  config.set({
      files: [
        { pattern: 'test-context.js', watched: false }
      ],
    frameworks : ['jasmine'],
    preprocessors: {
      'test-context.js': ['webpack', 'sourcemap']
    },
    webpack: {
      mode: 'development',
      devtool: 'inline-source-map',
      watch: true
    },
    browsers: ['Chrome'],
    concurrency: Infinity,
    singleRun: false
  });
};

// module.exports = function (config) {
//   config.set({
//     basePath: '',
//     frameworks: ['jasmine'],
//     files: [
//       // Vendor
//       '../web/vendor/jquery/jquery-3.2.1.min.js',
//       '../web/vendor/jquery/jquery.cookie.js',
//       '../web/vendor/jquery/jquery-ui.min.js',
//       '../web/vendor/popper/popper.js',
//       '../web/vendor/tether/tether.min.js',
//       '../web/vendor/bootstrap/js/bootstrap.js',
//       // Unit Test
//       'node_modules/jasmine-jquery/lib/jasmine-jquery.js',
//       // Application
//       '../web/js/md5.js',
//       { pattern: '../web/js/gerbera-app.module.js', type: 'module', included: true },
//       // '../web/js/gerbera.app.js',
//       // '../web/js/gerbera.auth.js',
//       // '../web/js/gerbera.items.js',
//       // '../web/js/gerbera.tree.js',
//       // '../web/js/gerbera.menu.js',
//       // '../web/js/jquery.gerbera.toast.js',
//       // '../web/js/gerbera.trail.js',
//       // '../web/js/gerbera.autoscan.js',
//       // '../web/js/gerbera.updates.js',
//       // '../web/js/jquery.gerbera.items.js',
//       // '../web/js/jquery.gerbera.tree.js',
//       // '../web/js/jquery.gerbera.trail.js',
//       // '../web/js/jquery.gerbera.editor.js',
//       // '../web/js/jquery.gerbera.autoscan.js',
//       // Tests
//       { pattern: 'test/client/gerbera.app.spec.js', type: 'module', included: true },
//       {
//         pattern: 'test/client/fixtures/**/*.json',
//         watched: true,
//         included: false,
//         served: true
//       },
//       {
//         pattern: 'test/client/fixtures/**/*.html',
//         watched: true,
//         included: false,
//         served: true
//       }
//     ],
//     exclude: [],
//     // preprocessors: {
//     //   '../web/js/*.js': 'coverage'
//     // },
//     reporters: ['mocha'],
//     // coverageReporter: {
//     //   type: 'html',
//     //   dir: 'coverage/'
//     // },
//     port: 9876,
//     colors: true,
//     logLevel: config.LOG_INFO,
//     autoWatch: false,
//     browsers: ['ChromeHeadless'],
//     concurrency: Infinity,
//     singleRun: false
//   })
// };
