# ML-Tools

一个简单的打表轮子，可枚举逻辑式各变量取值，并将真值表通过Markdown表格格式打印，可实现表达式去重，支持多个表达式同时打印与多组数据。



# 下载
[Latest release (TrueTable.exe)](https://github.com/RogerDTZ/MathematicalLogic/releases/tag/v1.1.1)



# 用法

- 1 - [前置环境](#1-前置环境)
- 2 - [运行](#2-运行)
- 3 - [输入格式](#3-输入格式)
  - 3.1 - [输入文件](#31-输入文件)
  - 3.2 - [表达式](#32-表达式)
  - 3.3 - [逻辑变量](#33-逻辑变量)
  - 3.4 - [逻辑运算符](#34-逻辑运算符)
- 4 - [输出说明](#4-输出说明)
- 5 - [示例](#5-示例)



## 1. 前置环境

你可能需要安装[VC运行库](https://aka.ms/vs/16/release/VC_redist.x64.exe)以运行本程序



## 2. 运行

本程序需要在命令行环境下运行

用法：

* `TrueTable.exe input_file  `：从`input_file`中读取数据，并将表格输出在命令行上
* `TrueTable.exe input_file output_file`：从`input_file`中读取数据，并将表格输出在`output_file`中



## 3. 输入格式

### 3.1 输入文件

在输入文件中输入数据。支持多组数据，每组数据之间应使用单行`---`隔开，最后一组数据不一定要附带`---`

### 3.2 表达式

每组数据中，一行一个目标表达式。表达式必须满足充分括号要求，即二元运算`AND` `OR`等表达式必须用一对括号包含，一元运算符`NEG`也须使用一对括号包含。多余括号是可接受的，且最外层表达式不要求使用括号包含

### 3.3 逻辑变量

逻辑变量名由大写字母`A~Z`、小写字母`a~z`、数字`0~9`和下划线`_`组成。逻辑变量数不应超过50个，反正多了也跑不出来

**注意：`T`与`F`被视作逻辑常量真和逻辑常量假，不能作为变量名**

### 3.4 逻辑运算符

逻辑运算符的输入方式如下：

|  negation  | conjunction | disjunction | nand | nor | material implication | equivalence |
| :----: | :----: | :---: | :----------------: | :--------------: | :-----------: | :---------------: |
| `\neg` | `\and` | `\or` | `\nand` 或  `\na`  | `\nor` 或 `\no`  |     `\im`     |       `\eq`       |

**注：每个运算符后应至少有一个空格，或紧跟左括号`(`**



## 4. 输出说明

表格采用Markdown样式，每列居中输出

先按输入出现的顺序依次输出底层逻辑变量，再输出各中间逻辑表达式，最后按输入的顺序依次输出目标表达式（但若目标表达式为一个底层逻辑变量，它不会被放到最后一列）。本质相同的逻辑表达式只会被输出一次

程序会枚举$2^N$种底层逻辑变量的取值，并输出所有中间逻辑表达式和目标表达式的取值



## 5. 示例

`exp.in`

```
(A \or B) \and (C \or D)
(D \or E)
(A \im (B \or C)) \or ((C \or D) \eq (D \or E))
---
(A \or B) \and (C \or D)
(D \or E)
(A \im (B \and C)) \or ((C \and D) \eq (D \or E))
---
(T \and A) \and (B \or F)
```

运行：`TrueTable.exe exp.in exp.out`

`exp.out`

```
| $A$ | $B$ | $C$ | $D$ | $E$ | $A \or B$ | $C \or D$ | $B \or C$ | $A \rightarrow (B \or C)$ | $(C \or D) \leftrightarrow (D \or E)$ | $(A \or B) \and (C \or D)$ | $D \or E$ | $(A \rightarrow (B \or C)) \or ((C \or D) \leftrightarrow (D \or E))$ |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| T | T | T | T | T | T | T | T | T | T | T | T | T |
| T | T | T | T | F | T | T | T | T | T | T | T | T |
| T | T | T | F | T | T | T | T | T | T | T | T | T |
| T | T | T | F | F | T | T | T | T | F | T | F | T |
| T | T | F | T | T | T | T | T | T | T | T | T | T |
| T | T | F | T | F | T | T | T | T | T | T | T | T |
| T | T | F | F | T | T | F | T | T | F | F | T | T |
| T | T | F | F | F | T | F | T | T | T | F | F | T |
| T | F | T | T | T | T | T | T | T | T | T | T | T |
| T | F | T | T | F | T | T | T | T | T | T | T | T |
| T | F | T | F | T | T | T | T | T | T | T | T | T |
| T | F | T | F | F | T | T | T | T | F | T | F | T |
| T | F | F | T | T | T | T | F | F | T | T | T | T |
| T | F | F | T | F | T | T | F | F | T | T | T | T |
| T | F | F | F | T | T | F | F | F | F | F | T | F |
| T | F | F | F | F | T | F | F | F | T | F | F | T |
| F | T | T | T | T | T | T | T | T | T | T | T | T |
| F | T | T | T | F | T | T | T | T | T | T | T | T |
| F | T | T | F | T | T | T | T | T | T | T | T | T |
| F | T | T | F | F | T | T | T | T | F | T | F | T |
| F | T | F | T | T | T | T | T | T | T | T | T | T |
| F | T | F | T | F | T | T | T | T | T | T | T | T |
| F | T | F | F | T | T | F | T | T | F | F | T | T |
| F | T | F | F | F | T | F | T | T | T | F | F | T |
| F | F | T | T | T | F | T | T | T | T | F | T | T |
| F | F | T | T | F | F | T | T | T | T | F | T | T |
| F | F | T | F | T | F | T | T | T | T | F | T | T |
| F | F | T | F | F | F | T | T | T | F | F | F | T |
| F | F | F | T | T | F | T | F | T | T | F | T | T |
| F | F | F | T | F | F | T | F | T | T | F | T | T |
| F | F | F | F | T | F | F | F | T | F | F | T | T |
| F | F | F | F | F | F | F | F | T | T | F | F | T |




| $A$ | $B$ | $C$ | $D$ | $E$ | $A \or B$ | $C \or D$ | $B \and C$ | $A \rightarrow (B \and C)$ | $C \and D$ | $(C \and D) \leftrightarrow (D \or E)$ | $(A \or B) \and (C \or D)$ | $D \or E$ | $(A \rightarrow (B \and C)) \or ((C \and D) \leftrightarrow (D \or E))$ |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| T | T | T | T | T | T | T | T | T | T | T | T | T | T |
| T | T | T | T | F | T | T | T | T | T | T | T | T | T |
| T | T | T | F | T | T | T | T | T | F | F | T | T | T |
| T | T | T | F | F | T | T | T | T | F | T | T | F | T |
| T | T | F | T | T | T | T | F | F | F | F | T | T | F |
| T | T | F | T | F | T | T | F | F | F | F | T | T | F |
| T | T | F | F | T | T | F | F | F | F | F | F | T | F |
| T | T | F | F | F | T | F | F | F | F | T | F | F | T |
| T | F | T | T | T | T | T | F | F | T | T | T | T | T |
| T | F | T | T | F | T | T | F | F | T | T | T | T | T |
| T | F | T | F | T | T | T | F | F | F | F | T | T | F |
| T | F | T | F | F | T | T | F | F | F | T | T | F | T |
| T | F | F | T | T | T | T | F | F | F | F | T | T | F |
| T | F | F | T | F | T | T | F | F | F | F | T | T | F |
| T | F | F | F | T | T | F | F | F | F | F | F | T | F |
| T | F | F | F | F | T | F | F | F | F | T | F | F | T |
| F | T | T | T | T | T | T | T | T | T | T | T | T | T |
| F | T | T | T | F | T | T | T | T | T | T | T | T | T |
| F | T | T | F | T | T | T | T | T | F | F | T | T | T |
| F | T | T | F | F | T | T | T | T | F | T | T | F | T |
| F | T | F | T | T | T | T | F | T | F | F | T | T | T |
| F | T | F | T | F | T | T | F | T | F | F | T | T | T |
| F | T | F | F | T | T | F | F | T | F | F | F | T | T |
| F | T | F | F | F | T | F | F | T | F | T | F | F | T |
| F | F | T | T | T | F | T | F | T | T | T | F | T | T |
| F | F | T | T | F | F | T | F | T | T | T | F | T | T |
| F | F | T | F | T | F | T | F | T | F | F | F | T | T |
| F | F | T | F | F | F | T | F | T | F | T | F | F | T |
| F | F | F | T | T | F | T | F | T | F | F | F | T | T |
| F | F | F | T | F | F | T | F | T | F | F | F | T | T |
| F | F | F | F | T | F | F | F | T | F | F | F | T | T |
| F | F | F | F | F | F | F | F | T | F | T | F | F | T |




| $A$ | $B$ | $T \and A$ | $B \or F$ | $(T \and A) \and (B \or F)$ |
| :-: | :-: | :-: | :-: | :-: |
| T | T | T | T | T |
| T | F | T | F | F |
| F | T | F | T | F |
| F | F | F | F | F |
```

渲染效果（Github readme中latex公式好像不渲染）：

| $A$ | $B$ | $C$ | $D$ | $E$ | $A \or B$ | $C \or D$ | $B \or C$ | $A \rightarrow (B \or C)$ | $(C \or D) \leftrightarrow (D \or E)$ | $(A \or B) \and (C \or D)$ | $D \or E$ | $(A \rightarrow (B \or C)) \or ((C \or D) \leftrightarrow (D \or E))$ |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| T | T | T | T | T | T | T | T | T | T | T | T | T |
| T | T | T | T | F | T | T | T | T | T | T | T | T |
| T | T | T | F | T | T | T | T | T | T | T | T | T |
| T | T | T | F | F | T | T | T | T | F | T | F | T |
| T | T | F | T | T | T | T | T | T | T | T | T | T |
| T | T | F | T | F | T | T | T | T | T | T | T | T |
| T | T | F | F | T | T | F | T | T | F | F | T | T |
| T | T | F | F | F | T | F | T | T | T | F | F | T |
| T | F | T | T | T | T | T | T | T | T | T | T | T |
| T | F | T | T | F | T | T | T | T | T | T | T | T |
| T | F | T | F | T | T | T | T | T | T | T | T | T |
| T | F | T | F | F | T | T | T | T | F | T | F | T |
| T | F | F | T | T | T | T | F | F | T | T | T | T |
| T | F | F | T | F | T | T | F | F | T | T | T | T |
| T | F | F | F | T | T | F | F | F | F | F | T | F |
| T | F | F | F | F | T | F | F | F | T | F | F | T |
| F | T | T | T | T | T | T | T | T | T | T | T | T |
| F | T | T | T | F | T | T | T | T | T | T | T | T |
| F | T | T | F | T | T | T | T | T | T | T | T | T |
| F | T | T | F | F | T | T | T | T | F | T | F | T |
| F | T | F | T | T | T | T | T | T | T | T | T | T |
| F | T | F | T | F | T | T | T | T | T | T | T | T |
| F | T | F | F | T | T | F | T | T | F | F | T | T |
| F | T | F | F | F | T | F | T | T | T | F | F | T |
| F | F | T | T | T | F | T | T | T | T | F | T | T |
| F | F | T | T | F | F | T | T | T | T | F | T | T |
| F | F | T | F | T | F | T | T | T | T | F | T | T |
| F | F | T | F | F | F | T | T | T | F | F | F | T |
| F | F | F | T | T | F | T | F | T | T | F | T | T |
| F | F | F | T | F | F | T | F | T | T | F | T | T |
| F | F | F | F | T | F | F | F | T | F | F | T | T |
| F | F | F | F | F | F | F | F | T | T | F | F | T |




| $A$ | $B$ | $C$ | $D$ | $E$ | $A \or B$ | $C \or D$ | $B \and C$ | $A \rightarrow (B \and C)$ | $C \and D$ | $(C \and D) \leftrightarrow (D \or E)$ | $(A \or B) \and (C \or D)$ | $D \or E$ | $(A \rightarrow (B \and C)) \or ((C \and D) \leftrightarrow (D \or E))$ |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| T | T | T | T | T | T | T | T | T | T | T | T | T | T |
| T | T | T | T | F | T | T | T | T | T | T | T | T | T |
| T | T | T | F | T | T | T | T | T | F | F | T | T | T |
| T | T | T | F | F | T | T | T | T | F | T | T | F | T |
| T | T | F | T | T | T | T | F | F | F | F | T | T | F |
| T | T | F | T | F | T | T | F | F | F | F | T | T | F |
| T | T | F | F | T | T | F | F | F | F | F | F | T | F |
| T | T | F | F | F | T | F | F | F | F | T | F | F | T |
| T | F | T | T | T | T | T | F | F | T | T | T | T | T |
| T | F | T | T | F | T | T | F | F | T | T | T | T | T |
| T | F | T | F | T | T | T | F | F | F | F | T | T | F |
| T | F | T | F | F | T | T | F | F | F | T | T | F | T |
| T | F | F | T | T | T | T | F | F | F | F | T | T | F |
| T | F | F | T | F | T | T | F | F | F | F | T | T | F |
| T | F | F | F | T | T | F | F | F | F | F | F | T | F |
| T | F | F | F | F | T | F | F | F | F | T | F | F | T |
| F | T | T | T | T | T | T | T | T | T | T | T | T | T |
| F | T | T | T | F | T | T | T | T | T | T | T | T | T |
| F | T | T | F | T | T | T | T | T | F | F | T | T | T |
| F | T | T | F | F | T | T | T | T | F | T | T | F | T |
| F | T | F | T | T | T | T | F | T | F | F | T | T | T |
| F | T | F | T | F | T | T | F | T | F | F | T | T | T |
| F | T | F | F | T | T | F | F | T | F | F | F | T | T |
| F | T | F | F | F | T | F | F | T | F | T | F | F | T |
| F | F | T | T | T | F | T | F | T | T | T | F | T | T |
| F | F | T | T | F | F | T | F | T | T | T | F | T | T |
| F | F | T | F | T | F | T | F | T | F | F | F | T | T |
| F | F | T | F | F | F | T | F | T | F | T | F | F | T |
| F | F | F | T | T | F | T | F | T | F | F | F | T | T |
| F | F | F | T | F | F | T | F | T | F | F | F | T | T |
| F | F | F | F | T | F | F | F | T | F | F | F | T | T |
| F | F | F | F | F | F | F | F | T | F | T | F | F | T |




| $A$ | $B$ | $T \and A$ | $B \or F$ | $(T \and A) \and (B \or F)$ |
| :-: | :-: | :-: | :-: | :-: |
| T | T | T | T | T |
| T | F | T | F | F |
| F | T | F | T | F |
| F | F | F | F | F |




