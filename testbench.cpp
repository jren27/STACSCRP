
#include <iostream>
#include <stack>
#include <gtest/gtest.h>
#include "src/diskstack.h"
#include "src/disk.h"

// DISKSTACK TESTS

TEST(DiskStackTest, pushPop) {
	DiskStack ds;
	Disk d;
	// Push/top tests
	for (int i = 0; i < 100; i++) {
		for (int i = 0; i < 100; i++) {
			d = Disk("!" + to_string(rand() % 100));
			ds.push(d);
			ASSERT_EQ(ds.getSize(), i + 1);
			EXPECT_EQ(ds.top(), d);
		}
		Disk topd, popd;
		for (int i = 100; i > 0; i--) {
			topd = ds.top();
			popd = ds.pop();
			EXPECT_EQ(topd, popd);
			ASSERT_EQ(ds.getSize(), i-1);
		}
	}
}

TEST(DiskStackTest, copyEquiv) {
	// Too lazy to make a fixture lol
	DiskStack ds1;
	Disk d;
	for (int i = 0; i < 100; i++) {
		d = Disk("!" + to_string(rand() % 100));
		ds1.push(d);
	}
	DiskStack ds2 = DiskStack(ds1);
	ASSERT_EQ(ds2 == ds1, true);
	ASSERT_EQ(ds2.getSize(), ds1.getSize());
	Disk pop1, pop2;
	while (ds1.getSize() != 0) {
		pop1 = ds1.pop();
		pop2 = ds2.pop();
		EXPECT_EQ(pop1, pop2);
	}
}
