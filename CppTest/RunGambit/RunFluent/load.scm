;;设置batch运行，如果出现错误自动退出，并隐藏错误文件
(ti-menu-load-string "/file/set-batch-options y y y n")
;;删除ffcerror
(system (string-append "del /s " "ffcerror"))

;;建立ffcerror文件，如果运行正常，在后期会删除该文件
(define port
  (open-output-file "ffcerror")
 )
(close-output-port port)

;(load '.\scheme\OpenCase.scm)
;(load '.\scheme\UserDefinedModels.scm)
;(load '.\scheme\CreateUDM.scm)
;(load '.\scheme\OpenUDFLibrary.scm)
;(load '.\scheme\Initialize.scm)
;(load '.\scheme\RunOnDemand.scm)
;(load '.\scheme\SetBC.scm)
;(display "end setBC")
;(load '.\scheme\SetGoafGasSource.scm)


;(display "end")
;;(load '.\scheme\SetIterate.scm)
;删除ffcerror
(system (string-append "del /s " "ffcerror"))
(exit)


