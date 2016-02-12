$fn=100;
difference()
{
union()
{
intersection()
{
	difference()
	{
		cylinder(h=12,r=25,center = false);
		translate([0,0,-1])
		{
			cylinder(h=15,r=21,center = false);
		}
	}
	translate([10,10,-1])
	{
		cube([30,30,12]);
	}
}
translate([11.5,20,1.5])
{
	rotate([-90,0,-30])
	{
		cylinder(h=6,r=1.5,center = false);
	}
}
translate([11.5,20,9.5])
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
translate([20,11.5,9.5])
{
	rotate([-90,0,-60])
	{
		cylinder(h=6,r=1.5,center = false);
	}
}
}
rotate([-90,0,-45])
{
	translate([0,-5.5,18])
	{
		cylinder(h=6,r=2.75,center = false);
	}
}
rotate([-90,0,-45])
{
	translate([1.5,-5.5,20])
	{
		cylinder(h=6,r=0.5,center = false);
	}
}
rotate([-90,0,-45])
{
	translate([-1.5,-5.5,20])
	{
		cylinder(h=6,r=0.5,center = false);
	}
}
}
