
int main(void)
{
	//readl(0x44E07000+0x134) | 0xffffff7f;
	writel(0xffffffbf, 0x4804C000+0x134);//oe bit6=0, output
	writel(0x0, 0x4804C000+0x13c);//gpio1_6, dataout, down; 0x0
	writel(0x40, 0x4804C000+0x13c);//gpio1_6, dataout, up; 0x40--->bit6=1
	return 0;
}


