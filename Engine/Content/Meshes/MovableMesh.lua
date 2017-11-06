return 
{
	-- Vertices

	
	vertices = 
	{
		-- Vertex 0
		{
			position =
			{
				x = -1.0,
				y = -1.0,
				z = 1.0,
			},
			color =
			{
				r = 1,
				g = 0,
				b = 0,
				a = 1,
			},
		},

		-- Vertex 1
		{
			position =
			{
				x = -1.0,
				y = 1.0,
				z = 1.0,
			},
			color =
			{
				r = 0,
				g = 1,
				b = 0,
				a = 1,
			},
		},

		-- Vertex 2
		{
			position =
			{
				x = 1.0,
				y = 1.0,
				z = 1.0,
			},
			color =
			{
				r = 0,
				g = 0,
				b = 1,
				a = 1,
			},
		},

		-- Vertex 3
		{
			position =
			{
				x = 1.0,
				y = -1.0,
				z = 1.0,
			},
			color =
			{
				r = 0,
				g = 1,
				b = 0,
				a = 1,
			},
		},

		-- Vertex 4
		{
			position =
			{
				x = -1.0,
				y = -1.0,
				z = -1.0,
			},
			color =
			{
				r = 1,
				g = 0,
				b = 0,
				a = 1,
			},
		},

		-- Vertex 5
		{
			position =
			{
				x = -1.0,
				y = 1.0,
				z = -1.0,
			},
			color =
			{
				r = 0,
				g = 1,
				b = 0,
				a = 1,
			},
		},

		-- Vertex 6
		{
			position =
			{
				x = 1.0,
				y = 1.0,
				z = -1.0,
			},
			color =
			{
				r = 0,
				g = 0,
				b = 1,
				a = 1,
			},
		},

		
		-- Vertex 7
		{
			position =
			{
				x = 1.0,
				y = -1.0,
				z = -1.0,
			},
			color =
			{
				r = 1,
				g = 1,
				b = 0,
				a = 1,
			},
		},
	},

	-- Indices
	indices = 
	{
		0 ,1 ,2, 0, 2, 3,
		2, 1, 5, 2, 5, 6,
		3, 2, 6, 3, 6, 7,
		4, 5, 1, 4, 1, 0,
		4, 0, 3, 4, 3, 7,
		4, 7, 6, 4, 6, 5,
	},


}