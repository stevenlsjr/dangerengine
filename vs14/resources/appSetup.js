/**
 * Created by Shea on 5/7/15.
 */



require(['dangerbird_bin', 'jquery'], function(engine, $){
    function main() {
        var Module = {};
        engine(Module);
    }

    $(document).ready(main);
});