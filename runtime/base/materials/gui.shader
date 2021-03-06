
textures/sylphis {
	technique standard {
		pass ambient {
			ambient
			bind texcoord0
			texture textures/system/sylphis.png			
			blend add
			depthtest false
		}
	}
}

gui/pointer {
	technique standard {
		pass ambient {
			bind texcoord0
			texture textures/system/pointer.png
			blend filter
			depthtest false
		}
	}
}

textures/testpart {

	transparent

	technique standard {
		pass ambient_ {
			ambient
			bind normal
			bind texcoord0
			shader shaders/particle.vp shaders/particle.fp
			sampler ambientTexture textures/particles/steam01.jpg
			uniform color [1.0, 1.0, 1.0]
			
			blend add
			//depthtest false
			depthwrite none
			cull none
		}
	}
}

textures/spark {

	transparent

	technique standard {
		pass ambient_ {
			ambient
			bind normal
			bind texcoord0
			shader shaders/particle.vp shaders/particle.fp
			sampler ambientTexture textures/particles/spark01.jpg
			uniform color [100.0, 100.0, 100.0]
			
			blend add
			//depthtest false
			depthwrite none
			cull none
		}
	}
}

textures/smoke {

	transparent

	technique standard {
		pass ambient_ {
			ambient
			bind normal
			bind texcoord0
			shader shaders/particle.vp shaders/particle.fp
			sampler ambientTexture textures/particles/steam01.jpg
			uniform color [10.0, 10.0, 10.0]
			
			blend add
			//depthtest false
			depthwrite none
			cull none
		}
	}
}