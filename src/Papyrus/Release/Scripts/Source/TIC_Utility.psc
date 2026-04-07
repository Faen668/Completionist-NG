scriptname TIC_Utility extends quest

TIC_MCM property mcm auto
;Other props you use a lot

Event OnInit()
	if (mcm == none)
		mcm = Quest.GetQuest("MCMQuest") as TIC_MCM
	endif
endEvent

TIC_MCM Function GetMCM()
	return mcm
endFunction

Function TestCompile()
	if (GetMCM() != None)
		return
	endif
endFunction
