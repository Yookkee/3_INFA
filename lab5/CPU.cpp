#include <iostream>
#include <ctime>

#define N 9
#define PASS 100
#define SUM_MAX_POS 2
#define SUM_MIN_POS 1
#define MUL_POS 6


class Programm{
public:
	void start_recurtion(void)
	{
		rec(0);
	}

	Programm(void)
	{
		found = 0;

		long long int *p = (long long int *)arr;
		long long int val = 0x0004000300020001;
		for (short i = 0; i < 2; i++){
			p[i] = val;
			val += 0x0004000400040004;
		}
		arr[8] = 9;
	}

	int get_found(void){
		return found;
	}
private:
	short arr[N];
	int found;

	void swap(short *a, short *b){
		short tmp = *a;
		*a = *b;
		*b = tmp;
	}

	void rec(short deep){
		short i;
		if (deep < N - 1){
			for (i = 0; i < N - deep; i++)
			{
				swap(&arr[i], &arr[N - deep - 1]);
				rec(deep + 1);
				swap(&arr[i], &arr[N - deep - 1]);
			}
		}
		else{
			int a, b, c;
			short sum, mul;
			for (sum = 1; sum < N - 2; sum++){
				for (mul = 2; mul < N - 1; mul++){
					if (mul <= sum) continue;

					a = b = c = 0;
					
					for (i = 0; i < sum; i++) a = a * 10 + arr[i];
					for (i = sum; i < mul; i++) b = b * 10 + arr[i];
					for (i = mul; i < N; i++) c = c * 10 + arr[i];

					if (a + ((float)b) / c == 100) found++;
				}
			}
		}
	}
};

int main()
{
	clock_t t = clock();
	class Programm Prog;
	for (int i = 0; i < 1000; i++) Prog.start_recurtion();
	std::cout << Prog.get_found();
	t = clock() - t;
	printf("\nIt took me %d clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);
	getchar();
}