bvulkan:
	cmake . -DBACKEND_VULKAN=ON -B build/vulkan
	make -C build/vulkan

bmetal:
	cmake . -DBACKEND_METAL=ON -B build/metal
	make -C build/metal
