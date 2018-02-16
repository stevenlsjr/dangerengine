/**
 * @module module.pre.js
 * defines emscripten object as a ecmascript module
 */

'use strict';
        const root = document.querySelector('#em-root');
        const tty = root.querySelector('#em-console');

        const inputForm = document.querySelector('form#em-input');
        const inputText = inputForm.querySelector('input#em-text');
        const inputSubmit = inputForm.querySelector('input#em-submit');

        inputSubmit.addEventListener('submit', (event) => {
            event.preventDefault();
        })

        var Module = {
            print(...args) {
                const node = document.createElement('div');
                node.textContent = args.join('');
                tty.appendChild(node);
            },
            printErr(...args) {
                const node = document.createElement('div');
                node.className = 'console-err';
                node.textContent = args.join('');
                tty.appendChild(node);
            }
        };
