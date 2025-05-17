### Todo
	Load meshes => HELL YEAH

	Mesh manager:
		store only once each mesh, and give a reference to Mesh component
		=> HELL YEAH
	
	Camera controls => HELL YEAH
	(almost)
	=> vertical (secondaire)
	=> zoom (très important)
	=> smarter movement system based on keyboard state (secondaire)

	Load and display textures => later

	MILESTONE New game object component: PointMass
		position, velocity
			use glm::dvec3 (vecor 3 of double)
		calculate gravity component (for others)
			make magnitude analysis to decide if we need to use double or long double
		calculate gravity (check all other game objects that have pointmass component, get their gravity component, apply acceleration (from sum of all components) to self)
			O(n²)
			is worth optimizing when n ~ 500 ? (grid makes it O(n*log(n)) iirc)

		position in transform component is currently glm::vec3. on update cast pointmass position to vec3 and pass it to transform for synchronicity
	Handle physics:
		when initializing, if we spawn in a PointMass, activate physics
		before updating our entities through ECS, call a physics function
			this function will find all entities that have a pointmass component and store a reference of/pointer to them
			then for each entity compute acceleration based on their own gravcomp, mass of target, coordinates of both entities
				for first entity, compute all but self
				for second entity, do not compute self and first (for the first has already computed it)
				...

				reset dacc for all entites, then:
				for i=0, i<L
					for j=i+1, i<L
						accEl = ...
						E[i]->dacc += accEl;
						E[j]->dacc -= accEl;
			
			then rk4 (also write 2,8? for testing) on speed, position

			done?

	File based entity loading (CSV style: name, mass, initial position, initial velocity, radius)

	Make textblock based ui => yeah dont really care
		(fontsize is equal to line size? if so we can easily place things vertically)
