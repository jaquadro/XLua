// --------------------------------
// Condition menu
// --------------------------------

#ifdef CONDITION_MENU

#endif

// --------------------------------
// Action menu
// --------------------------------

#ifdef ACTION_MENU

	// You have access to edPtr and you can throw in normal C++
	// code wherever you want in any of these menus. If you want to
	// show certain conditions based on a property (a la OINC socket)
	// use the property's value stored in edPtr:

	// if(edPtr->ShowAdvancedOptions)
	// {
	//		SUB_START("Bake pie")
	//			ITEM(0, "Apple pie")
	//			ITEM(1, "Mince pie")
	//		SUB_END
	// }
	// else
	// {
	//		ITEM(0, "Bake apple pie")
	// }

	// You could do some pretty cool stuff with this, like loading a
	// menu from an external source (ActiveX object?)

	// for(int i=0;i<edPtr->ActionCount;i++)
	//		ITEM(i,edPtr->Actions[i])

	// This will run every time the menu is opened in MMF2, so don't
	// make it too slow or it could get annoying

	SUB_START("Scale / Angle")
		ITEM(1,"Set Scale")
		ITEM(2,"Set X Scale")
		ITEM(3,"Set Y Scale")
		ITEM(0,"Set Angle")
	SUB_END
	SEPARATOR
	SUB_START("Hot Spot")
		ITEM(4,"Custom...")
		SEPARATOR
		ITEM(5,"Top-left")
		ITEM(6,"Top-center")
		ITEM(7,"Top-right")
		ITEM(8,"Left-center")
		ITEM(9,"Center")
		ITEM(10,"Right-center")
		ITEM(11,"Bottom-left")
		ITEM(12,"Bottom-center")
		ITEM(13,"Bottom-right")
	SUB_END
	//SEPARATOR
	//ITEM(2,"Set Width")
	//ITEM(3,"Set Height")

#endif


// --------------------------------
// Expression menu
// --------------------------------

#ifdef EXPRESSION_MENU

	SEPARATOR
	ITEM(0,"Get Width")
	ITEM(1,"Get Height")

#endif