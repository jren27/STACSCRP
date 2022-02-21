
#include <iostream>
#include <gtest/gtest.h>
#include "src/diskstack.h"
#include "src/disk.h"

// DISKSTACK TESTS

TEST(DiskStackTest, pushPop) {
	DiskStack ds;
	Disk d("LTRL", "");
	string value;
	for (int i = 0; i < 100; i++) {
		value = "!" + to_string(rand() % 100);
		d.setValue(value);
		ds.push(d);
		EXPECT_EQ(ds.getSize(), i + 1);
		EXPECT_EQ(ds.top(), d);
	}
}
