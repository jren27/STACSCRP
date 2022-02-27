
#include <iostream>
#include <stack>
#include <gtest/gtest.h>
#include "src/diskstack.h"
#include "src/disk.h"

// --- DISKSTACK TESTS ---

TEST(DiskStack, pushPop) {
	DiskStack ds;
	Disk d;
	// Push/top tests
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			d = Disk("!" + to_string(rand() % 100));
			ds.push(d);
			ASSERT_EQ(ds.getSize(), j + 1);
			ASSERT_EQ(ds.top(), d);
		}
		Disk topd, popd;
		for (int j = 100; j > 0; j--) {
			topd = ds.top();
			popd = ds.pop();
			ASSERT_EQ(topd, popd);
			ASSERT_EQ(ds.getSize(), j-1);
		}
	}
}

TEST(DiskStack, copyEquiv) {
	// Too lazy to make a fixture lol
	for (int i = 0; i < 100; i++) {
		DiskStack ds1;
		Disk d;
		for (int j = 0; j < 100; j++) {
			d = Disk("!" + to_string(rand() % 100));
			ds1.push(d);
		}
		DiskStack ds2 = DiskStack(ds1);
		ASSERT_EQ(ds2.getSize(), ds1.getSize());
		Disk pop1, pop2;
		while (ds1.getSize() != 0) {
			pop1 = ds1.pop();
			pop2 = ds2.pop();
			ASSERT_EQ(ds2 == ds1, true);
			ASSERT_EQ(pop1, pop2);
		}
	}
}

TEST(DiskStack, swap) {
	Disk topswap = Disk("top");
	for (int i = 0; i < 100; i++) {	
		DiskStack ds;
		Disk d;
		ds.push(topswap);
		for (int j = 0; j < 100; j++) {
			d = Disk("!" + to_string(rand() % 100));
			ds.push(d);
			ASSERT_EQ(ds.top(), d);
			ds.swap();
			ASSERT_EQ(ds.top(), topswap);
		}
	}
}

// --- DISK TESTS ---

TEST(Disk, fillDisk) {
	Disk d, fillDisk;
	for (int i = 0; i < 100; i++) {
		d = Disk("NULL", false);
		ASSERT_EQ(d.getValue(), "NULL");
		ASSERT_EQ(d.isLiteral(), false);
		for (int j = 0; j < 100; j++) {
			fillDisk = Disk("!" + to_string(rand() % 100));
			d.getStack()->push(fillDisk);
			ASSERT_EQ(d.getStack()->top(), fillDisk);
			ASSERT_EQ(d.getStack()->getSize(), j + 1);
		}
	}
}
