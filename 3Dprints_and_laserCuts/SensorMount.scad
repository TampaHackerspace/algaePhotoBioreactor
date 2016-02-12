$fn=100;
intersection()
{
	difference()
	{
		cylinder(h=10,r=25,center = false);
		translate([0,0,-1])
		{
			cylinder(h=15,r=21,center = false);
		}
	}
	translate([10,10,-1])
	{
		cube([30,30,10]);
	}
}
translate([11.5,20,1.5])
{
	rotate([-90,0,-30])
	{
		cylinder(h=6,r=1.5,center = false);
	}
}
translate([11.5,20,7.5])
{
	rotate([-90,0,-30])
	{
		cylinder(h=6,r=1.5,center = false);
	}
}
translate([20,11.5,1.5])
{
	rotate([-90,0,-60])
	{
		cylinder(h=6,r=1.5,center = false);
	}
}
translate([20,11.5,7.5])
{
	rotate([-90,0,-60])
	{
		cylinder(h=6,r=1.5,center = false);
	}
}