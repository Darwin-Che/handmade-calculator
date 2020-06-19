var http = require('http');

http.createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/html' });
    res.write("tokenlist is " + evalrpn( shuntyard( tokenize(0, "1 - 2 * 3") ) ));
    res.end();
}).listen(1003);

// tokenize 

tokenize = function (i, str) {
    var token_list = [];
    var arr_str = str.split("");
    var l = str.length;
    while (i < l) {
        // console.log(i);
        var ch = str.charAt(i);
        if (islp(ch)) {
            token_list.push('(');
            ++i;
        } else if (isrp(ch)) {
            token_list.push(')');
            ++i;
        } else if (isspace(ch)) {
            ++i;
        } else if (isnum(ch) || ch == '0') {
            var ret = readnum(i, str);
            i = ret[0];
            token_list.push(ret[1]);
        } else {
            var ret = readop(i, str);
            i = ret[0];
            token_list.push(ret[1]);
            should_be_num = 0;
        }
    }
    return token_list;
}

readnum = function (i, str) {
    var ans_int = 0; // int part of ans
    var ans_dbl = 0; // decimal part of ans if float
    var dbl_digit = 1;
    var float = 0; // 0: int; 1: float
    var neg = 0; // 0: pos; 1:neg
    if (str.charAt(i) == '-') {
        neg = 1;
        ++i;
    }
    while (isnum(str.charAt(i))) {
        if (str.charAt(i) == '.') {
            // we cannot have two '.'
            if (float == 1) {
                throw new Error("invalid number " + ans + '.');
            }
            float = 1;
        } else if (float == 0) {
            // readin integer part
            ans_int = ans_int * 10 + str.charCodeAt(i) - 48;
        } else if (float == 1) {
            // readin digits after period
            ans_dbl = ans_dbl + (str.charAt(i) - '0') * Math.pow(10, -dbl_digit);
            dbl_digit++;
        }
        ++i;
    }
    var ret = [i, (float == 0) ? ans_int : ans_int + ans_dbl];
    if (neg == 1) ret[1] = -ret[1];
    return ret;
}

readop = function (i, str) {
    if (str.charAt(i) == '+') {
        var ret = [i + 1, '+'];
        return ret;
    } else if (str.charAt(i) == '-') {
        var ret = [i + 1, '-'];
        return ret;
    } else if (str.charAt(i) == '*') {
        var ret = [i + 1, '*'];
        return ret;
    } else if (str.charAt(i) == '/') {
        var ret = [i + 1, '/'];
        return ret;
    } else if (str.charAt(i) == '^') {
        var ret = [i + 1, '^'];
        return ret;
    } else if ((str.charAt(i) == 'l') && (str.charAt(i + 1) == 'o') && (str.charAt(i + 2) == 'g')) {
        var ret = [i + 3, 'log'];
        return ret;
    } else {
        throw new Error("invalid operator " + str.charAt(i));
    }
}

islp = function (ch) {
    return (ch == '(') || (ch == '[') || (ch == '{');
}

isrp = function (ch) {
    return (ch == ')') || (ch == ']') || (ch == '}');
}

isnum = function (ch) {
    return ((ch >= '0') && (ch <= '9')) || (ch == '.');
}

isspace = function (ch) {
    return (ch == ' ') || (ch == '\n');
}

// convert to rpn

isop = function (str) {
    all_op = ['+', '-', '*', '/', '^', 'log'];
    return all_op.indexOf(str) != -1;
}

if_pop = function (input_op, stack_op) {
    switch (input_op) {
        case '+':
        case '-':
            switch (stack_op) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '^':
                case 'log':
                    return true;
                default:
                    throw new Error("invalid Op" + stack_op);
            }
        case '*':
        case '/':
            switch (stack_op) {
                case '+':
                case '-':
                    return 0;
                case '*':
                case '/':
                case '^':
                case 'log':
                    return 1;
                default:
                    throw new Error("invalid Op" + stack_op); 
            }
        case '^':
            switch (stack_op) {
                case '+':
                case '-':
                case '*':
                case '/':
                    return 0;
                case '^':
                case 'log':
                    return 1;
                default:
                    throw new Error("invalid Op" + stack_op); 
            }
        case 'log':
            switch (stack_op) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '^':
                case 'log':
                    return 0;
                default:
                    throw new Error("invalid Op" + stack_op);
            }
        
        default:
            throw new Error("invalid Op" + input_op);
    }
}

shuntyard = function (in_lst) {
    // shuntingyard stacks
    var input_lst = in_lst; // we donot need the original copy anymore
    var op_stack = [];
    var output_lst = [];
    var i = 0;
    while (i < input_lst.length) {
        var input_elem = input_lst[i];
        // console.log(input_elem);
        // console.log("opstack:" + op_stack);
        // console.log("ooutput:" + output_lst);
        if (input_elem == '(') {
            op_stack.push('(');
            ++i;
        } else if (input_elem == ')') {
            while (op_stack[op_stack.length - 1] != '(') {
                if (op_stack.length == 1) {
                    throw new Error("mismatched parenthesis!\n");
                }
                output_lst.push(op_stack.pop());
            }
            op_stack.pop();
            ++i;
        } else if ((typeof input_elem) == "number") {
            output_lst.push(input_elem);
            ++i;
        } else if (isop(input_elem)) {
            // complicate logic, reference https://en.wikipedia.org/wiki/Shunting-yard_algorithm
            while ( (op_stack.length > 0) 
                    && isop(op_stack[op_stack.length - 1]) 
                    && if_pop(input_elem, op_stack[op_stack.length - 1]) ) {
                output_lst.push(op_stack.pop());
            }
            op_stack.push(input_elem);
            ++i;
        } else {
            throw new Error("invalid token type!\n");
        }
    }
    while (op_stack.length > 0) {
        var op_elem = op_stack.pop();
        if (op_elem == '(' || op_elem == ')') {
            throw new Error("mismatched parenthesis.\n");
        } else {
            output_lst.push(op_elem);
        }
    }
    return output_lst;
}

// evaluate rpn

eval_op_binary = function (n2, n1, op) {
    switch (op) {
        case '+':
            return n1 + n2;
        case '-':
            return n1 - n2;
        case '*':
            return n1 * n2;
        case '/':
            return n1 / n2;
        case '^':
            return Math.pow(n1, n2);
        default:
            throw new Error("invalid binary operator: " + op);
    }
}

eval_op_unary = function (n, op) {
    switch (op) {
        case 'log':
            return Math.log(n);
        default: 
            throw new Error("invalid unary operator");
    }
}

evalrpn = function (rpn_lst) {
    var eval_res = []; // store the temporary results and the final result also appears here
    var i = 0;
    var l = rpn_lst.length;
    while (i < l) {
        var rpn_elem = rpn_lst[i];
        if (typeof rpn_elem == "number") {
            eval_res.push(rpn_elem);
        } else {
            switch (rpn_elem) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '^':
                    if (eval_res.length < 2) {
                        throw new Error("not enough operand for " + rpn_elem);
                    }
                    var res = eval_op_binary(eval_res.pop(), eval_res.pop(), rpn_elem);
                    eval_res.push(res);
                    break;
                case 'log':
                    if (eval_res.length < 1) {
                        throw new Error("not enough operand for " + rpn_elem);
                    }
                    var res = eval_op_unary(eval_res.pop(), rpn_elem);
                    eval_res.push(res);
                    break;
                default:
                    throw new Error("invalid operator " + rpn_elem);
            }
        }
        ++i;
    }
    if (eval_res.length > 1 || eval_res.length < 1) {
        throw new Error("After evaluation left with more than a number: " + eval_res);
    }
    return eval_res[0];
}