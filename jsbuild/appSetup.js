/**
 * Created by Shea on 5/7/15.
 */

requirejs.config({
  baseUrl: "/",
  paths: {
      'jquery': 'bower/jquery/dist/jquery'
  }
});


require(['dangerbird_bin','jquery'], function(engine, $){
  'use strict'
  function main() {
    var Module = {};
    Module.canvas = $('#canvas').get()[0];
    console.log(Module);


    engine(Module);
  }

  $(document).ready(main);
});