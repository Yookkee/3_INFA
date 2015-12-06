
__kernel void test(__global int* result)
{
	int sum_pos = get_global_id(1) + 1;
	int div_pos = get_global_id(2) + 2;
	if (sum_pos >= div_pos) return;

	int gid = get_global_id(0);

	int arr[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};

	int i;
	for (i = 8; i > 0; i--)
	{
		int work = gid % (i + 1);

		int tmp = arr[work];
		arr[work] = arr[i];
		arr[i] = tmp;

		gid /= (i + 1);
	}

	int a, b, c;
	a = b = c = 0;
 
	for (i = 0; i < sum_pos; i++) a = a * 10 + arr[i];
	for (i = sum_pos; i < div_pos; i++) b = b * 10 + arr[i];
	for (i = div_pos; i < 9; i++) c = c * 10 + arr[i];

	if (a + (float)b / c == 100)
	{ 
		atomic_inc(result);
	}
}

