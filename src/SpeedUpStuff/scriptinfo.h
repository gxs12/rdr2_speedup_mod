/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))


// 449 = animal drop from shoulder
//
//453, start pickup + removing from stowed horse
//454, = moving to pickup location
//455, picking up

//444, skinning + gathering plant
//445, start skinning and skinning

//457, stowing back of horse

//367 - pulling lasso'ed object

//452  = hogtieing
//454  = start hogtie

// 4 = ??? dismount
// 185 = ??? mount
const static int cTaskIdSkips[] {
	444, // skinning + plant gathering
	445, // start skinning + start plant gathering
	449, // animal drop from shoulder (i think dropping anything)
	452, // while in hogtie
	453, // start pickup + removing stowed object from horse
	454, // moving to pickup location + start hogtie
	455, // picking up
	457 // stowing back of horse
	//367 - pulling object that has lasso
};