1、将lang文件夹放至以下目录，目录结构如下：
├─lang
│  ├─zh-CN
│  │      Globallang_zh-CN.xml
│  │
│  └─zh-TW
│          Globallang_zh-TW.xml
│
└─plugins
        Global.fpi
        InstallGlobal.xml
		
2、多语言xml中的说明
1）<Dialogs>标签是用于多语言Dialog窗口的
2）dlgitem是对应Dialog窗口中的每一个控件，ID对应控件的ID号
3）<Strings> 标签是用于多语言字符ID的，该ID对应Resource View中 String Table中的ID
