# markdown_pro说明

## 1. 标签支持能力

1. 支持的元素：
    * 主要针对块级元素进行解析，包含p, h1-h6, ul, ol, blockquote 
    * 內联元素只处理了code 

2. 常规markdown內联元素的支持不包含在此实现中，原因是基于lex & yacc对语法的严格要求，不适合markdown语法的随意性，会出现非常多的syntax error，而error handling很不容易实现。
    比如內联元素a, img等，编写时，直接写其HTML格式，如下：

        <a href="url">title</a>
        <img src="url">

3. 支持多级嵌套列表

4. 支持嵌套代码段

6. 其他HTML标签，直接编写，如：
        <table><tr><td>1</td></tr></table>

## 2. 语法说明 

### 2.1 h1-h6

语法：

    # h1 text 
    ## h2 text
    ### h3 text
    #### h4 text
    ##### h5 text
    ###### h6 text

会产生以下输出：

    <h1>h1 text</h1>
    <h2>h2 text</h2>
    <h3>h3 text</h3>
    <h4>h4 text</h4>
    <h5>h5 text</h5>
    <h6>h6 text</h6>

### 2.2 常规段落

