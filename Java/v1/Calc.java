package v1;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.io.*;

class Op {
	String operator;
	int level;
	int oplevel;

	Op(String x, int oplevel_tmp) {
		operator = x;
		oplevel = oplevel_tmp;
		if (x.equals("^"))
			level = 2;
		if (x.equals("*"))
			level = 1;
		if (x.equals("/"))
			level = 1;
		if (x.equals("+"))
			level = 0;
		if (x.equals("-"))
			level = 0;
	}

	boolean compareTo(Op tmp) {
		if (this.oplevel > tmp.oplevel)
			return true;
		if (this.oplevel < tmp.oplevel)
			return false;
		if (this.level > tmp.level)
			return true;
		return false;
	}
}

public class Calc {
	public Map<String, Double> math_list = new HashMap<String, Double>();
	{
		{
			math_list.put("pi", Math.PI);
			math_list.put("e", Math.E);
		}
	}
	public String s = "";
	public List<Op> ops = new ArrayList<Op>();
	public List<Double> nums = new LinkedList<Double>();

	public void break_s(String s) {
		char ch;
		String double_tmp = "";
		int op_level = 0;
		String math_tmp = "";
		for (int i = 0; i < s.length(); i++) {
			ch = s.charAt(i);
			switch (ch) {
			case ' ':
				break;
			default:
				math_tmp = math_tmp + String.valueOf(ch);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
				double_tmp = double_tmp + ch;
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
				ops.add(new Op(String.valueOf(ch), op_level));
				if (!double_tmp.equals(""))
					nums.add(Double.valueOf(double_tmp));
				if (!math_tmp.equals(""))
					nums.add(math_list.get(math_tmp));
				double_tmp = "";
				math_tmp = "";
				break;
			case '[':
			case '{':
			case '(':
				op_level++;
				break;
			case ')':
			case ']':
			case '}':
				op_level--;
				break;
			}
		}
		if (!double_tmp.equals(""))
			nums.add(Double.valueOf(double_tmp));
		if (!math_tmp.equals(""))
			nums.add(math_list.get(math_tmp));
	}

	public void calc() {
		int x = find_p();
		nums.set(x, calc_imple(nums.get(x), ops.remove(x), nums.remove(x + 1)));
	}

	public Double calc_imple(Double d1, Op op, Double d2) {
		Double ans = Double.valueOf(0);
//		System.out.println(d1== null);
		switch (op.operator) {
		case "+":
			ans = Double.valueOf(d1.doubleValue() + d2.doubleValue());
			break;
		case "-":
			ans = Double.valueOf(d1.doubleValue() - d2.doubleValue());
			break;
		case "*":
			ans = Double.valueOf(d1.doubleValue() * d2.doubleValue());
			break;
		case "/":
			ans = Double.valueOf(d1.doubleValue() / d2.doubleValue());
			break;
		case "^":
			ans = Double.valueOf(Math.pow(d1.doubleValue(), d2.doubleValue()));
		}
		return ans;
	}

	public int find_p() {
		int ans = 0;
		for (int tmp = 1; tmp < ops.size(); tmp++) {
			if (ops.get(tmp).compareTo(ops.get(ans)))
				ans = tmp;
		}
		return ans;

	}

	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		Calc calc = new Calc();
		calc.break_s(br.readLine());
		while (calc.ops.size() > 0) {
			calc.calc();
		}
		System.out.println(calc.nums.get(0));
//		for (Op x : calc.ops)
//			System.out.print(x.operator + x.oplevel + "  ");
//		System.out.println();
//		for (Double n : calc.nums)
//			System.out.print(n.doubleValue() + "  ");
	}
}

