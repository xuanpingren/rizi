秦历
----
[ 为什么 ]

为什么要开发这个日历程序？回答是这样的。 农历这个东西千家万户天天都在用，
影响深而远，波及范围广，中国有人口约14 0000 0000。 农历，只此中国一家，
别无分店。 农历是先进历法，拥有现行公历的长处（主要体现在节气），而且记
载月亮的阴晴圆缺。 农历十五，为望，月圆。农历初一，为朔，月亏。不是已经
有日历程序可以显示农历了吗？是的。但是，以农历为主要显示以公历为辅助显
示的程序则不多见。并且，现在程序的显示方式主要为7列表格式，不利于形象表
示24节气与一个月的周期。秦历尝试用圆周的方式来表示日子的迁移，或许可以
弥补这个不足。


[ 源码简述 ]

从index.html读入用户数据，比如起迄时间，是否显示公历。然后qinli.c处理之。
get_form_input 这个函数把用户数据读入，写到叫做form的结构中。见94行和
734行。 qinli.c主要做转换的工作，就是把目前的计算机时间换成农历。
qinli.c打印出一个漂亮网页，用函数printf。点击秦历按钮后，就会调用一系列
的printf，输出一个网页。


[ 测试 ]

上面步骤完成后，把ql.cgi与index.html复制到Apache默认主页存放位置。 打开
浏览器，在地址栏输入127.0.0.1/index.html，就可以运行了。测试主要可以用
prinf打印，然后刷新index.html看效果与输出。


[ 上交代码 ]

用 git 自行提交到 <b>kaifa 分支</b>。流程参考
https://github.com/xuanpingren/rizi/wiki/ 下的
使用git协作工作流程页面）
