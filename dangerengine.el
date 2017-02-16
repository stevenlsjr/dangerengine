;;; package --- dangerengine emacs config
;;; Commentary:
;;; Steve Shea 2017
;;; dangerengine project.el
;;; Emacs config for dangerengine
;;;
;;; Code:

(require 'auto-complete)
(require 'flycheck)

(defun dangerengine-c-hook
    ()
  "C config for project."
  (local-set-key [C-M-tab] 'clang-format-region)
  (set-variable flycheck-clang-include-path
                (append flycheck-clang-include-path
                        ))
  (auto-complete-mode 1))

(add-hook 'c-mode-hook #'dangerengine-c-hook)

(provide 'dangerengine)
;;; dangerengine.el ends here
