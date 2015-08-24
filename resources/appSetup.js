/**
 * Created by Shea on 5/7/15.
 */


require(['demo', 'jquery'], function(engine, $){
    function main() {
        var Module = {};
        engine(Module);
    }

    $(document).ready(main);
});