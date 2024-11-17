

comps = sorted("BAS COM LCR SIG SYS TSW".split())
mods = sorted("Provider Dispatcher Analayzer Checker Driver Supporter".split())

for comp in comps:
    for mod in mods:
        for i in range(1, 6):
            print(f"X:/git/dstw/application/components/{comp}/{comp}_{mod}_{i:02d}.h:")
    for mod in mods:
        for i in range(1, 6):
            print(f"X:/git/dstw/application/components/{comp}/src/{comp}_{mod}_{i:02d}.cpp:")
