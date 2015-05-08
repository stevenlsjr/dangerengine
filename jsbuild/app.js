/**
 * Created by Shea on 5/7/15.
 */
define(function (require){
    var $ = require('jquery'),
        dangerbird = require('dangerbird_bin');

    function main() {
        var module = $('#canvas').get();
        dangerbird(module);
    }

    $(document).ready(main);
});