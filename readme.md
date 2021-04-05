# ML-Tools
一个简单的打表轮子，可枚举逻辑式各变量取值，并将真值表通过Markdown表格格式打印，可实现表达式去重，支持多个表达式同时打印与多组数据。



# Download
[Latest release](https://github.com/RogerDTZ/MathematicalLogic/releases/tag/1.0)



# Usage

## 1. 前置环境

你可能需要安装[VC运行库](https://aka.ms/vs/16/release/VC_redist.x64.exe)以运行本程序

## 2. 运行

本程序需要在命令行环境下运行

用法：

* `TrueTable.exe inputfile  `：从`inputfile`中读取数据，并将表格输出在命令行上
* `TrueTable.exe inputfile outputfile`：从`inputfile`中读取数据，并将表格输出在`output_file`中

## 3. 输入格式

输入支持多组数据，每组数据之间应使用单行`---`隔开，**不同组数据之间不应存在多余空行**

每组数据中，一行一个目标表达式。表达式必须满足充分括号要求，即二元运算`AND` `OR`等表达式必须用一对括号包含，一元运算符`NEG`也须使用一对括号包含。多余括号是可接受的，且最外层表达式不要求使用括号包含

逻辑变量名由大写字母`A~Z`、小写字母`a~z`、数字`0~9`和下划线`_`组成

逻辑运算符的输入方式如下：

|  negation  | conjunction | disjunction | nand | nor | material implication | equivalence |
| :----: | :----: | :---: | :----------------: | :--------------: | :-----------: | :---------------: |
| `\neg` | `\and` | `\or` | `\nand` 或  `\na`  | `\nor` 或 `\no`  |     `\im`     |       `\eq`       |

**注：每个运算符后应至少有一个空格，或紧跟左括号`(`**

## 4. 输出说明

表格采用Markdown样式，每列居中输出

先按输入出现的顺序依次输出底层逻辑变量，再输出各中间逻辑表达式，最后按输入的顺序依次输出目标表达式（但若目标表达式为一个底层逻辑变量，它不会被放到最后一列）。本质相同的逻辑表达式只会被输出一次。

程序会枚举$2^N$种底层逻辑变量的取值，并输出所有中间逻辑表达式和目标表达式的取值