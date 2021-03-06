#include "../traversal.h"
#include "../storage.h"

#include <gtest/gtest.h>

TEST(Traversal, Outmost) {
	auto storage = new chunked_voxel_storage<bit_t>(0., 0., 0., 0.1, 10, 10, 10, 10);

	for (size_t i = 2; i < 7; ++i) {
		for (size_t j = 2; j < 7; ++j) {
			for (size_t k = 2; k < 7; ++k) {
				if (((make_vec(4, 4, 4) - make_vec(i, j, k).as<int>()).abs() < 2).all()) {
					continue;
				}
				storage->Set({ i, j, k });
			}
		}
	}

	ASSERT_EQ(storage->count(), 5 * 5 * 2 + 5 * 3 * 2 + 3 * 3 * 2);
	
	storage->Set({ 4U,4U,4U });

	ASSERT_EQ(storage->count(), 5 * 5 * 2 + 5 * 3 * 2 + 3 * 3 * 2 + 1);

	auto leftmost = query_leftmost()(storage);
	ASSERT_EQ(leftmost.get(0), 2);

	int count = 0;
	visitor<>()([&count](const tagged_index&) {
		count++;
	}, storage, leftmost);

	auto outmost = keep_outmost()(storage);
	ASSERT_EQ(outmost->count(), 5 * 5 * 2 + 5 * 3 * 2 + 3 * 3 * 2);
}
