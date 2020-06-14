; this is a comment
cutom-objects
{
	camera
	{
		properties
		{
			pos { type vec3 }
			ori { type quat }
		}
		components
		{
			view { id main prio 0 pos pos ori ori }
			lookat { target box source pos prop ori}
		}
	}
	box
	{
		properties
		{
			pos { type vec3 }
		}
		components
		{
			render-cube { size 1 texture crate01a.jpg pos pos }
			movement { script demomovement.gm }
			input { script wasd.gm }
		}
	}
}

sky
{
	type box
	texture gray.png
}

objects
{
	demogrid {}
	box
	{
		pos { x 0 y 0 z 0 }
	}
	camera
	{
		pos { x 0 y 0 z 0 }
	}
}