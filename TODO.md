### Todo
	Load meshes => HELL YEAH

	Mesh manager:
		store only once each mesh, and give a reference to Mesh component
	
	Camera controls

	Load and display textures

	MILESTONE New game object component: PointMass
		position, velocity
			use glm::dvec3 (vecor 3 of double)
		calculate gravity component (for others)
			make magnitude analysis to decide if we need to use double or long double
		calculate gravity (check all other game objects that have pointmass component, get their gravity component, apply acceleration (from sum of all components) to self)
			O(n²)
			is worth optimizing when n ~ 500 ? (grid makes it O(n*log(n)) iirc)

		position in transform component is currently glm::dvec3. maybe pass it to glm:vec3 so all graphics side shit is float,
		and have pointmass store a glm::dvec3 position that is each loop casted and passed to transform
	
	File based entity loading

	Make textblock based ui
		(fontsize is equal to line size? if so we can easily place things vertically)
