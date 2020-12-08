#include <catch2/catch.hpp>

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

namespace {

auto constexpr test_input =
    R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.
)";

auto constexpr another_test = R"(shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.
)";

auto constexpr input =
    R"(dotted blue bags contain 5 wavy green bags, 3 pale beige bags.
dull lime bags contain 1 dotted olive bag, 3 dim brown bags.
mirrored magenta bags contain 3 mirrored gray bags, 2 plaid beige bags, 4 dull brown bags, 3 pale plum bags.
posh coral bags contain 2 light blue bags, 2 dotted purple bags, 1 pale fuchsia bag, 2 light cyan bags.
bright orange bags contain 2 shiny chartreuse bags.
plaid salmon bags contain 1 faded coral bag, 4 clear lavender bags, 5 wavy tan bags.
wavy lavender bags contain 2 drab purple bags, 2 light brown bags, 2 wavy tomato bags, 1 mirrored plum bag.
clear indigo bags contain 4 dark magenta bags, 2 shiny brown bags.
posh salmon bags contain 4 dull tomato bags, 3 drab black bags, 5 mirrored chartreuse bags, 4 shiny blue bags.
mirrored coral bags contain 4 faded aqua bags, 2 plaid plum bags, 5 striped tan bags, 5 wavy tomato bags.
dotted purple bags contain 1 faded gold bag.
bright purple bags contain 2 muted indigo bags, 3 pale maroon bags, 4 dim coral bags, 2 dark blue bags.
drab olive bags contain 2 shiny coral bags.
dim orange bags contain 1 faded turquoise bag.
drab aqua bags contain 1 faded black bag, 4 clear lavender bags.
bright salmon bags contain 2 bright aqua bags.
dim brown bags contain 1 bright yellow bag, 3 faded cyan bags, 4 dotted chartreuse bags.
clear tan bags contain 2 posh tomato bags, 4 bright tan bags, 1 mirrored gray bag.
faded beige bags contain 2 mirrored olive bags, 5 dark chartreuse bags.
drab chartreuse bags contain 1 shiny white bag.
faded coral bags contain 4 dotted green bags, 2 dim violet bags, 3 striped magenta bags.
vibrant crimson bags contain 5 shiny gray bags, 2 muted black bags, 2 posh brown bags.
mirrored aqua bags contain 5 bright brown bags, 3 mirrored tomato bags, 3 bright bronze bags, 1 shiny magenta bag.
dotted salmon bags contain 2 muted tan bags, 4 light bronze bags.
vibrant salmon bags contain 5 dull yellow bags, 5 wavy tan bags, 3 light fuchsia bags, 1 drab tan bag.
vibrant olive bags contain 2 pale white bags, 2 dull coral bags, 1 mirrored magenta bag, 2 clear crimson bags.
shiny plum bags contain 3 dotted chartreuse bags, 1 striped tomato bag, 4 clear indigo bags.
pale fuchsia bags contain 1 mirrored indigo bag, 2 plaid aqua bags, 1 striped teal bag, 4 drab salmon bags.
wavy beige bags contain 1 mirrored coral bag, 1 bright plum bag, 2 dark lavender bags.
drab silver bags contain 5 vibrant gold bags, 5 pale gray bags.
light salmon bags contain 2 drab chartreuse bags, 3 faded gray bags, 3 pale gray bags, 1 clear olive bag.
faded red bags contain 5 dull chartreuse bags, 2 plaid aqua bags.
clear maroon bags contain 2 muted lavender bags.
dull green bags contain 3 pale olive bags, 5 vibrant gold bags, 2 clear teal bags, 5 dotted plum bags.
striped white bags contain 1 dim cyan bag.
pale coral bags contain 5 dim purple bags, 3 faded silver bags, 3 dim blue bags, 3 dotted brown bags.
drab green bags contain 5 pale teal bags.
posh tomato bags contain 4 drab fuchsia bags, 5 dark brown bags.
shiny chartreuse bags contain 5 dim maroon bags, 5 dull indigo bags, 5 pale purple bags, 3 vibrant gold bags.
dark plum bags contain 4 muted tan bags.
faded green bags contain 4 drab lavender bags.
pale tomato bags contain 3 bright maroon bags, 4 pale turquoise bags, 4 dark gold bags, 3 dull indigo bags.
striped aqua bags contain 1 dull gray bag, 5 dim blue bags, 5 wavy cyan bags.
dotted lavender bags contain 4 pale plum bags, 2 bright coral bags.
light gold bags contain 3 light tan bags, 4 clear indigo bags, 3 shiny tan bags.
striped maroon bags contain 5 drab tan bags, 1 pale black bag.
plaid black bags contain 3 dull fuchsia bags.
dim beige bags contain 1 dim teal bag, 1 muted salmon bag.
mirrored teal bags contain 2 bright coral bags, 4 dotted brown bags, 5 vibrant white bags, 3 clear magenta bags.
faded gold bags contain no other bags.
faded yellow bags contain 1 faded brown bag, 3 clear tomato bags.
bright violet bags contain 1 faded lime bag, 5 dark chartreuse bags, 1 shiny black bag, 3 vibrant yellow bags.
shiny fuchsia bags contain 2 dotted plum bags.
dim aqua bags contain 1 dim cyan bag.
dotted fuchsia bags contain 5 muted blue bags, 1 pale brown bag, 2 shiny salmon bags.
dim fuchsia bags contain 3 muted tomato bags, 2 striped lime bags, 2 clear white bags, 2 striped blue bags.
faded teal bags contain 1 wavy gold bag, 1 clear coral bag, 3 pale crimson bags.
dull plum bags contain 1 bright tan bag, 5 light violet bags, 5 shiny black bags.
light cyan bags contain 1 mirrored turquoise bag, 1 dim gray bag, 5 faded white bags.
vibrant cyan bags contain 1 dotted gold bag, 1 mirrored indigo bag.
bright coral bags contain 1 pale lavender bag, 3 mirrored olive bags, 3 muted tan bags.
dim chartreuse bags contain 3 pale white bags, 5 faded white bags, 3 mirrored magenta bags, 5 faded black bags.
pale green bags contain 3 faded white bags, 5 dotted lavender bags.
wavy gray bags contain 3 wavy tomato bags, 4 dull gray bags, 4 shiny teal bags, 5 pale beige bags.
dotted cyan bags contain 4 bright white bags.
shiny olive bags contain 2 wavy chartreuse bags, 5 dim chartreuse bags, 4 pale gold bags, 1 posh crimson bag.
drab maroon bags contain 3 clear olive bags, 4 dotted blue bags, 4 dim magenta bags.
striped purple bags contain 4 wavy beige bags, 1 pale orange bag, 1 pale brown bag.
dull olive bags contain 2 vibrant yellow bags.
faded tomato bags contain 4 light turquoise bags.
dotted maroon bags contain 2 faded cyan bags, 3 muted tomato bags, 1 shiny aqua bag, 3 bright orange bags.
wavy turquoise bags contain 1 drab magenta bag.
plaid green bags contain 1 light blue bag, 1 mirrored silver bag, 5 striped gold bags.
drab indigo bags contain 5 dull aqua bags.
plaid crimson bags contain 1 posh yellow bag, 5 shiny cyan bags.
drab salmon bags contain 3 bright lavender bags, 4 bright violet bags, 1 mirrored brown bag, 4 shiny gold bags.
dim turquoise bags contain 5 wavy brown bags, 4 mirrored yellow bags.
pale bronze bags contain 2 dotted bronze bags, 5 dim purple bags, 1 plaid red bag.
striped blue bags contain 5 drab fuchsia bags.
wavy aqua bags contain 5 posh plum bags, 1 striped bronze bag, 4 dark magenta bags.
posh beige bags contain 1 faded lime bag, 3 wavy white bags, 2 mirrored gray bags.
plaid magenta bags contain 4 light olive bags, 3 faded salmon bags.
plaid gray bags contain 1 light indigo bag, 5 muted indigo bags.
mirrored white bags contain 3 muted gray bags, 4 dotted beige bags, 5 dotted salmon bags, 5 muted gold bags.
dull chartreuse bags contain 3 muted gray bags.
faded cyan bags contain 4 light violet bags, 4 faded lime bags, 2 bright silver bags, 3 pale olive bags.
mirrored red bags contain 1 dotted violet bag, 5 bright plum bags, 2 pale violet bags, 4 pale plum bags.
shiny purple bags contain 2 pale magenta bags, 2 shiny fuchsia bags.
vibrant plum bags contain 5 light bronze bags, 3 plaid lavender bags, 1 shiny coral bag, 2 vibrant yellow bags.
shiny lime bags contain 1 posh black bag.
mirrored salmon bags contain 5 dull magenta bags, 2 clear gray bags, 1 dim indigo bag, 4 light brown bags.
dotted violet bags contain 2 pale violet bags.
muted orange bags contain 5 dark coral bags, 3 mirrored tomato bags, 1 dotted green bag.
bright lime bags contain 3 shiny gold bags, 4 plaid lavender bags, 3 dull gold bags.
pale lavender bags contain no other bags.
clear aqua bags contain 2 plaid turquoise bags.
clear white bags contain 4 shiny coral bags, 5 striped gold bags, 2 mirrored magenta bags.
clear chartreuse bags contain 1 posh lime bag.
wavy silver bags contain 3 clear orange bags, 2 clear lavender bags.
posh cyan bags contain 1 shiny crimson bag.
light plum bags contain 1 plaid lavender bag.
dim lime bags contain 5 bright crimson bags, 3 dark green bags, 2 vibrant green bags, 3 pale maroon bags.
dim coral bags contain 4 muted crimson bags, 1 mirrored plum bag.
pale salmon bags contain 5 wavy orange bags, 4 dotted gold bags, 4 posh brown bags.
faded chartreuse bags contain 1 dotted chartreuse bag, 3 wavy orange bags.
faded olive bags contain 5 pale blue bags, 3 dim yellow bags, 1 dotted gray bag, 2 dim crimson bags.
clear lime bags contain 3 dull fuchsia bags, 4 wavy red bags.
bright blue bags contain 3 mirrored violet bags, 3 pale gold bags, 4 drab salmon bags, 5 dull silver bags.
muted maroon bags contain 5 dull salmon bags, 3 dim gray bags.
clear teal bags contain 2 muted tomato bags, 4 faded turquoise bags.
shiny white bags contain 1 posh red bag, 3 dotted red bags.
light magenta bags contain 4 drab olive bags.
vibrant coral bags contain 5 pale lavender bags, 5 plaid red bags.
wavy yellow bags contain 3 vibrant beige bags, 4 muted crimson bags, 4 bright white bags, 1 pale maroon bag.
bright black bags contain 2 striped maroon bags.
plaid lavender bags contain 1 drab turquoise bag.
light crimson bags contain 2 wavy chartreuse bags.
muted salmon bags contain 2 bright orange bags, 3 dotted plum bags, 3 light magenta bags.
striped teal bags contain 4 dark chartreuse bags, 1 dim maroon bag.
dull bronze bags contain no other bags.
dotted bronze bags contain 1 striped orange bag, 5 shiny cyan bags, 2 muted tan bags.
dark gold bags contain 3 pale plum bags, 3 mirrored olive bags.
mirrored lime bags contain 3 dim maroon bags, 3 light violet bags, 2 mirrored turquoise bags, 3 vibrant yellow bags.
clear violet bags contain 2 drab salmon bags, 4 pale gray bags, 4 striped tan bags, 5 mirrored blue bags.
plaid beige bags contain 3 dull gold bags.
shiny salmon bags contain 3 dark chartreuse bags, 4 bright yellow bags, 5 dark gold bags, 2 muted tomato bags.
dull brown bags contain 1 bright silver bag, 4 muted tan bags.
dull orange bags contain 1 dim bronze bag.
vibrant lavender bags contain 4 dotted violet bags, 3 striped lime bags.
light blue bags contain 2 shiny teal bags, 5 striped orange bags, 5 shiny gray bags.
plaid teal bags contain 4 dark turquoise bags, 4 posh indigo bags, 1 mirrored tan bag.
drab black bags contain 4 vibrant chartreuse bags, 4 dim turquoise bags.
light beige bags contain 3 pale aqua bags.
mirrored gray bags contain 5 dark chartreuse bags.
faded black bags contain 4 shiny salmon bags.
light yellow bags contain 1 dotted magenta bag.
bright red bags contain 5 bright bronze bags, 3 dull white bags, 3 dim plum bags, 3 light tan bags.
mirrored lavender bags contain 5 faded chartreuse bags.
shiny coral bags contain 3 plaid chartreuse bags, 1 pale lavender bag.
clear bronze bags contain 4 mirrored silver bags, 1 dim coral bag, 3 posh teal bags.
dotted yellow bags contain 1 bright crimson bag, 5 dotted olive bags.
light bronze bags contain 4 dim maroon bags, 3 shiny crimson bags.
wavy tomato bags contain 5 dull red bags, 3 wavy cyan bags, 4 dark gold bags.
dotted turquoise bags contain 5 muted silver bags.
posh orange bags contain 2 bright beige bags, 5 vibrant chartreuse bags, 3 drab green bags.
wavy bronze bags contain 1 dark coral bag.
plaid tan bags contain 1 vibrant bronze bag.
wavy red bags contain 4 dotted gray bags, 2 pale purple bags, 1 bright tan bag.
dull turquoise bags contain 4 dull crimson bags.
faded brown bags contain 5 muted teal bags.
dark violet bags contain 1 dim gold bag, 1 dim beige bag, 5 dull orange bags.
bright brown bags contain 1 plaid aqua bag, 4 light cyan bags.
dull teal bags contain 4 shiny teal bags.
dotted white bags contain 3 pale black bags, 2 drab black bags, 5 faded fuchsia bags, 1 plaid black bag.
dark magenta bags contain 5 shiny teal bags, 4 pale purple bags.
mirrored maroon bags contain 3 plaid chartreuse bags, 2 muted orange bags, 5 pale gray bags, 4 drab green bags.
faded magenta bags contain 1 bright green bag, 5 pale lavender bags, 4 posh fuchsia bags, 2 pale teal bags.
wavy violet bags contain 4 bright indigo bags.
posh gold bags contain 1 bright green bag, 3 dim cyan bags.
muted silver bags contain 3 light lime bags, 4 wavy indigo bags, 5 striped turquoise bags, 5 clear blue bags.
muted fuchsia bags contain 2 pale olive bags, 1 light maroon bag, 3 plaid blue bags, 1 light blue bag.
posh turquoise bags contain 3 light lime bags, 4 wavy tomato bags, 3 vibrant blue bags, 3 posh fuchsia bags.
dull purple bags contain 1 striped tan bag, 2 vibrant tomato bags, 2 dim brown bags, 2 shiny fuchsia bags.
light fuchsia bags contain 2 mirrored turquoise bags, 4 striped lime bags, 4 faded white bags.
dull gold bags contain 5 plaid tan bags, 2 light bronze bags, 1 striped tan bag.
wavy olive bags contain 4 vibrant maroon bags, 1 drab salmon bag.
pale beige bags contain 1 wavy white bag, 1 dotted orange bag, 4 dim gray bags.
clear crimson bags contain 4 dull blue bags.
dotted tomato bags contain 5 dotted olive bags.
drab purple bags contain 1 striped tan bag, 3 mirrored olive bags, 4 mirrored turquoise bags.
plaid red bags contain 2 bright cyan bags, 5 pale lavender bags, 3 dark chartreuse bags, 1 dotted plum bag.
posh silver bags contain 5 light olive bags, 5 bright white bags, 5 light violet bags, 4 faded orange bags.
muted teal bags contain 2 wavy violet bags.
light red bags contain 2 dull white bags, 4 drab fuchsia bags.
bright crimson bags contain 4 light lavender bags, 2 dotted plum bags.
plaid purple bags contain 5 dull magenta bags, 5 light lavender bags.
dark gray bags contain 4 dull plum bags, 5 muted tomato bags, 3 drab brown bags.
vibrant gray bags contain 4 posh black bags.
plaid fuchsia bags contain 1 posh lime bag.
striped chartreuse bags contain 4 posh teal bags, 1 bright purple bag, 1 clear white bag.
dull fuchsia bags contain 3 bright brown bags, 3 striped olive bags, 2 faded white bags.
muted beige bags contain 4 drab chartreuse bags.
vibrant beige bags contain 4 striped crimson bags.
vibrant indigo bags contain 5 dull plum bags, 1 dull maroon bag, 2 striped white bags.
dull silver bags contain 5 dull white bags, 2 dull gold bags, 4 shiny cyan bags, 4 wavy gold bags.
dim blue bags contain 5 posh yellow bags, 5 dim maroon bags.
posh white bags contain 4 dim teal bags, 4 light brown bags.
mirrored tomato bags contain 1 posh purple bag, 3 striped aqua bags.
light black bags contain 1 mirrored chartreuse bag, 3 muted yellow bags, 4 muted tan bags.
plaid silver bags contain 2 dull beige bags, 5 bright tan bags, 5 posh tomato bags.
shiny violet bags contain 1 plaid beige bag, 3 faded teal bags, 4 dotted tomato bags, 1 shiny orange bag.
dull black bags contain 5 vibrant coral bags, 2 clear orange bags.
dim white bags contain 5 drab brown bags, 2 plaid lavender bags.
wavy salmon bags contain 5 dull white bags, 5 vibrant crimson bags, 2 posh silver bags, 1 dim purple bag.
dark green bags contain 1 dull magenta bag, 3 light tomato bags, 1 pale olive bag, 5 pale brown bags.
light chartreuse bags contain 1 dotted silver bag, 3 bright maroon bags, 4 vibrant silver bags, 5 bright coral bags.
dim green bags contain 3 wavy bronze bags, 3 bright tan bags, 1 dotted magenta bag.
shiny tan bags contain 3 striped turquoise bags, 4 drab brown bags, 2 wavy tan bags, 3 light fuchsia bags.
muted lime bags contain 4 dark yellow bags, 5 vibrant blue bags.
dark coral bags contain 5 plaid chartreuse bags.
striped fuchsia bags contain 4 light lavender bags, 4 dull gray bags, 5 wavy cyan bags, 2 striped orange bags.
shiny maroon bags contain 3 striped olive bags, 5 dull green bags, 4 shiny cyan bags.
dotted red bags contain 5 light violet bags, 4 plaid chartreuse bags.
dim salmon bags contain 1 dim blue bag, 3 dotted indigo bags, 4 pale crimson bags, 3 drab blue bags.
pale magenta bags contain 5 pale violet bags, 1 dark blue bag.
clear coral bags contain 2 faded turquoise bags, 2 shiny cyan bags, 4 dull red bags.
dim black bags contain 4 dim aqua bags, 5 pale blue bags.
clear cyan bags contain 2 light violet bags, 5 drab salmon bags, 4 dark teal bags.
dull gray bags contain 2 bright lavender bags, 3 pale plum bags, 5 dull plum bags.
dull salmon bags contain 4 posh violet bags, 3 mirrored indigo bags, 3 plaid tomato bags.
pale cyan bags contain 2 pale chartreuse bags, 4 light brown bags, 1 plaid beige bag, 3 light maroon bags.
faded indigo bags contain 3 dim maroon bags, 3 mirrored violet bags.
wavy orange bags contain 3 plaid purple bags.
dim plum bags contain 2 wavy tomato bags.
pale turquoise bags contain 2 faded lime bags.
drab orange bags contain 2 mirrored turquoise bags, 5 plaid tomato bags, 3 dotted blue bags, 5 wavy red bags.
mirrored cyan bags contain 5 muted chartreuse bags, 4 dim cyan bags, 5 posh fuchsia bags.
vibrant purple bags contain 5 dotted turquoise bags.
posh red bags contain 4 faded turquoise bags, 2 light violet bags.
muted crimson bags contain 5 shiny fuchsia bags.
drab magenta bags contain 1 dull fuchsia bag.
wavy tan bags contain 1 mirrored plum bag.
vibrant tomato bags contain 3 plaid blue bags, 3 clear fuchsia bags, 5 dark yellow bags.
mirrored gold bags contain 3 muted purple bags, 5 dark indigo bags, 5 dotted brown bags, 1 mirrored olive bag.
mirrored plum bags contain 3 faded turquoise bags.
muted gray bags contain 4 light bronze bags.
light olive bags contain 3 bright lavender bags, 4 wavy orange bags, 3 clear lavender bags.
faded tan bags contain 4 vibrant red bags, 1 drab bronze bag, 5 mirrored plum bags.
posh plum bags contain 4 dotted gold bags, 4 dotted plum bags, 2 vibrant gold bags.
faded maroon bags contain 5 dim red bags.
drab teal bags contain 5 faded chartreuse bags, 2 faded silver bags.
clear black bags contain 2 pale red bags, 3 plaid black bags.
mirrored indigo bags contain 3 dim chartreuse bags, 1 wavy violet bag, 1 dull green bag.
pale teal bags contain 4 wavy indigo bags.
drab cyan bags contain 4 dotted plum bags, 5 dim red bags.
striped salmon bags contain 4 clear olive bags, 4 wavy orange bags.
faded bronze bags contain 5 mirrored olive bags, 2 dark green bags.
muted magenta bags contain 2 mirrored beige bags, 1 wavy orange bag, 3 light salmon bags.
wavy plum bags contain 5 light cyan bags, 2 drab turquoise bags.
dark lavender bags contain 2 dim coral bags.
faded lavender bags contain 4 wavy bronze bags, 3 dim yellow bags.
dark maroon bags contain 5 striped orange bags.
clear fuchsia bags contain 1 dull brown bag, 4 posh red bags, 3 plaid purple bags.
shiny magenta bags contain 2 striped maroon bags, 5 mirrored olive bags, 2 drab lime bags.
wavy coral bags contain 4 posh maroon bags, 5 dim white bags, 5 pale indigo bags, 5 dotted maroon bags.
dark silver bags contain 3 posh plum bags.
shiny blue bags contain 1 wavy indigo bag, 4 dark magenta bags, 4 light turquoise bags.
clear lavender bags contain 1 bright violet bag, 4 wavy cyan bags, 2 dim blue bags, 5 posh violet bags.
shiny orange bags contain 2 pale tomato bags, 1 pale teal bag.
mirrored beige bags contain 5 dotted gold bags, 1 faded violet bag, 4 striped gray bags.
bright cyan bags contain 4 muted tomato bags, 1 drab turquoise bag, 4 bright yellow bags.
vibrant fuchsia bags contain 4 striped tan bags, 1 dark black bag, 4 plaid olive bags, 2 wavy aqua bags.
pale olive bags contain 4 posh yellow bags, 4 pale plum bags.
dotted crimson bags contain 3 dim gray bags, 1 mirrored lime bag.
vibrant orange bags contain 5 striped orange bags, 3 plaid blue bags.
light brown bags contain 4 dotted chartreuse bags.
muted plum bags contain 1 muted indigo bag, 5 plaid yellow bags, 1 vibrant silver bag, 1 drab turquoise bag.
dotted chartreuse bags contain 5 dotted red bags, 5 light lavender bags, 4 pale black bags, 3 muted tomato bags.
bright chartreuse bags contain 5 dim salmon bags, 5 shiny salmon bags, 4 bright plum bags, 2 dark lavender bags.
dim gray bags contain 3 dull indigo bags.
faded fuchsia bags contain 2 muted blue bags, 1 mirrored blue bag, 2 dotted green bags, 5 mirrored olive bags.
striped tomato bags contain 4 dull gray bags, 1 vibrant plum bag, 3 mirrored coral bags.
clear turquoise bags contain 4 light orange bags, 3 faded black bags, 5 mirrored lavender bags, 5 posh white bags.
dotted gray bags contain 4 mirrored turquoise bags, 5 light lavender bags, 1 vibrant yellow bag, 4 dull bronze bags.
shiny beige bags contain 3 plaid silver bags, 2 faded chartreuse bags, 2 vibrant lavender bags, 2 wavy olive bags.
pale red bags contain 4 mirrored olive bags.
mirrored silver bags contain 2 vibrant maroon bags, 5 plaid chartreuse bags, 4 vibrant plum bags, 5 faded white bags.
plaid aqua bags contain 5 faded aqua bags, 4 dotted fuchsia bags.
dull yellow bags contain 5 bright turquoise bags, 3 posh plum bags, 1 pale white bag.
faded orange bags contain 3 dotted bronze bags, 1 mirrored tan bag.
wavy green bags contain 1 wavy turquoise bag.
posh tan bags contain 5 striped lavender bags, 4 posh tomato bags.
wavy crimson bags contain 4 pale lavender bags, 2 mirrored tan bags, 5 plaid beige bags.
striped yellow bags contain 3 shiny green bags, 5 dark gold bags, 4 drab teal bags, 3 drab olive bags.
pale plum bags contain 2 dull indigo bags, 1 dotted plum bag, 1 bright yellow bag.
pale yellow bags contain 2 bright green bags, 5 dull beige bags, 3 light coral bags, 3 dull red bags.
striped silver bags contain 1 clear crimson bag, 4 shiny salmon bags.
bright silver bags contain 2 shiny black bags, 3 faded gold bags, 5 bright yellow bags.
shiny red bags contain 1 pale turquoise bag, 5 shiny beige bags.
posh chartreuse bags contain 5 dotted maroon bags, 5 shiny bronze bags, 1 shiny cyan bag, 3 pale aqua bags.
wavy teal bags contain 4 mirrored plum bags, 4 drab silver bags, 3 pale green bags, 1 mirrored lime bag.
posh olive bags contain 2 dim salmon bags, 1 faded black bag, 1 light lime bag.
dark fuchsia bags contain 3 mirrored indigo bags.
dark cyan bags contain 4 muted indigo bags, 4 shiny crimson bags, 2 drab turquoise bags.
posh magenta bags contain 5 bright blue bags, 1 dull turquoise bag.
muted red bags contain 3 light violet bags, 2 dark cyan bags.
vibrant gold bags contain 2 dotted chartreuse bags, 5 pale brown bags, 1 mirrored brown bag, 5 plaid lavender bags.
muted purple bags contain 3 dim blue bags, 3 wavy red bags, 1 clear lavender bag.
dim magenta bags contain 5 muted tan bags, 4 striped olive bags, 5 vibrant gold bags.
faded plum bags contain 1 faded black bag, 1 muted green bag.
light violet bags contain 3 mirrored turquoise bags, 1 dull bronze bag.
faded salmon bags contain 1 pale salmon bag, 1 light teal bag.
posh black bags contain 2 wavy purple bags.
plaid white bags contain 1 shiny salmon bag, 3 posh purple bags.
mirrored green bags contain 3 bright tan bags, 1 plaid white bag, 3 mirrored plum bags.
dark red bags contain 2 bright purple bags, 2 muted white bags, 5 drab cyan bags, 5 plaid olive bags.
vibrant teal bags contain 5 striped olive bags, 1 pale olive bag.
dotted green bags contain 3 dark cyan bags.
dark indigo bags contain 3 bright brown bags, 3 drab white bags, 2 muted purple bags, 2 plaid silver bags.
drab tan bags contain 3 dotted gray bags, 2 faded black bags, 1 dull brown bag, 2 drab salmon bags.
vibrant chartreuse bags contain 5 pale brown bags.
mirrored olive bags contain 2 dim blue bags, 2 shiny gold bags, 4 light lavender bags.
light silver bags contain 3 drab orange bags, 5 light plum bags, 2 dotted olive bags.
light indigo bags contain 4 pale yellow bags, 1 vibrant violet bag, 4 dull cyan bags, 5 dotted tomato bags.
pale orange bags contain 4 pale silver bags, 4 vibrant tomato bags.
vibrant bronze bags contain 5 dark yellow bags.
dull beige bags contain 4 dotted gray bags, 4 pale lavender bags, 5 light lavender bags, 3 shiny coral bags.
bright gray bags contain 3 bright silver bags.
vibrant green bags contain 4 muted tan bags, 3 light lavender bags, 5 light magenta bags.
plaid violet bags contain 4 bright lime bags, 2 mirrored olive bags, 4 faded blue bags.
striped bronze bags contain 1 plaid chartreuse bag.
dim crimson bags contain 3 bright yellow bags, 1 dull gray bag.
posh purple bags contain 3 light fuchsia bags, 5 pale black bags, 5 wavy orange bags, 3 drab blue bags.
clear beige bags contain 4 plaid black bags.
striped gray bags contain 2 pale green bags, 4 light fuchsia bags.
plaid olive bags contain 2 mirrored gray bags.
muted green bags contain 1 plaid coral bag.
clear salmon bags contain 4 muted black bags, 1 bright white bag, 1 drab tomato bag, 2 shiny orange bags.
dark aqua bags contain 5 light bronze bags, 1 wavy turquoise bag.
drab tomato bags contain 5 dim chartreuse bags.
wavy magenta bags contain 3 posh brown bags.
dull crimson bags contain 1 mirrored tan bag, 4 dotted crimson bags, 5 vibrant indigo bags.
plaid turquoise bags contain 1 dark bronze bag, 4 bright maroon bags.
dim cyan bags contain 4 plaid red bags, 4 bright silver bags, 5 vibrant gold bags.
faded blue bags contain 3 plaid salmon bags, 3 clear cyan bags, 1 wavy aqua bag, 4 mirrored teal bags.
light orange bags contain 4 posh chartreuse bags, 3 shiny maroon bags, 4 dim teal bags, 4 drab chartreuse bags.
bright white bags contain 5 drab chartreuse bags.
clear red bags contain 1 dull magenta bag, 1 light cyan bag.
drab gold bags contain 4 plaid violet bags, 5 muted gold bags.
muted chartreuse bags contain 1 plaid crimson bag, 4 vibrant red bags, 1 drab turquoise bag, 5 posh black bags.
mirrored black bags contain 1 dim white bag, 1 clear gold bag.
bright indigo bags contain 1 vibrant gold bag, 2 pale purple bags.
muted indigo bags contain 2 dull plum bags.
striped tan bags contain 2 dotted chartreuse bags.
dim tan bags contain 2 dim red bags, 4 light violet bags, 1 clear beige bag.
muted tomato bags contain 1 faded gold bag, 2 bright yellow bags, 3 dotted plum bags.
vibrant turquoise bags contain 2 light tan bags, 4 wavy salmon bags, 1 dark tan bag, 1 dull tan bag.
dotted magenta bags contain 4 plaid blue bags, 1 plaid turquoise bag, 5 striped silver bags.
dark orange bags contain 3 faded bronze bags.
light tomato bags contain 2 dull bronze bags, 1 pale brown bag, 4 bright violet bags.
dim teal bags contain 5 dull brown bags.
muted lavender bags contain 4 dark yellow bags, 4 shiny teal bags, 2 faded aqua bags, 5 wavy cyan bags.
bright beige bags contain 3 muted black bags, 4 bright blue bags.
light purple bags contain 1 pale lavender bag, 2 shiny aqua bags.
plaid blue bags contain 4 dull beige bags, 3 dull brown bags, 3 dotted crimson bags.
striped lavender bags contain 3 dotted chartreuse bags.
bright tomato bags contain 1 posh violet bag, 3 shiny aqua bags.
wavy indigo bags contain 4 mirrored olive bags, 1 faded turquoise bag, 1 vibrant coral bag, 4 bright gray bags.
pale black bags contain 5 bright silver bags, 3 mirrored turquoise bags.
mirrored turquoise bags contain no other bags.
bright plum bags contain 2 faded turquoise bags.
shiny yellow bags contain 1 clear gold bag.
plaid bronze bags contain 1 dotted teal bag, 3 faded white bags, 2 shiny green bags.
striped beige bags contain 3 vibrant teal bags.
wavy cyan bags contain 5 posh yellow bags.
light tan bags contain 2 vibrant red bags.
pale brown bags contain 2 bright yellow bags, 4 pale lavender bags, 5 mirrored turquoise bags, 1 drab turquoise bag.
bright teal bags contain 3 dark green bags, 2 dull white bags.
drab yellow bags contain 5 dotted purple bags.
shiny cyan bags contain 4 vibrant yellow bags.
vibrant silver bags contain 5 mirrored yellow bags, 5 dull indigo bags, 3 faded cyan bags, 4 faded turquoise bags.
clear plum bags contain 4 pale lavender bags.
posh teal bags contain 2 muted tan bags.
striped crimson bags contain 1 dull plum bag, 2 shiny fuchsia bags, 4 dim maroon bags, 3 bright yellow bags.
dull lavender bags contain 2 clear fuchsia bags, 5 pale silver bags, 5 dim salmon bags, 4 muted salmon bags.
posh indigo bags contain 5 faded chartreuse bags, 5 dark green bags, 5 dark blue bags.
wavy blue bags contain 5 muted yellow bags.
dull magenta bags contain 5 pale brown bags.
mirrored bronze bags contain 3 posh turquoise bags.
posh aqua bags contain 1 dim white bag, 2 clear tomato bags, 2 pale maroon bags.
pale purple bags contain 2 dotted fuchsia bags.
mirrored chartreuse bags contain 5 plaid yellow bags, 5 posh tan bags, 4 vibrant silver bags, 3 drab green bags.
dotted black bags contain 5 muted aqua bags, 1 wavy lime bag, 2 bright lime bags.
plaid chartreuse bags contain 2 bright yellow bags, 2 faded gold bags, 1 pale lavender bag.
dotted plum bags contain 4 dull bronze bags, 4 dim blue bags.
clear gold bags contain 2 bright tan bags, 5 wavy teal bags.
dark crimson bags contain 4 bright teal bags, 2 wavy gray bags, 1 bright chartreuse bag.
faded gray bags contain 2 mirrored magenta bags.
dim gold bags contain 3 drab white bags.
clear green bags contain 2 pale beige bags.
dotted gold bags contain 3 dull indigo bags.
striped lime bags contain 2 dotted chartreuse bags.
clear brown bags contain 4 dark olive bags, 2 striped yellow bags, 4 dotted red bags, 5 dotted plum bags.
striped gold bags contain 1 wavy white bag, 3 posh tomato bags, 4 light magenta bags.
muted brown bags contain 1 clear gold bag.
plaid tomato bags contain 4 plaid plum bags, 2 pale chartreuse bags.
dark salmon bags contain 5 pale plum bags, 3 striped lime bags, 4 dotted gray bags.
drab brown bags contain 1 wavy orange bag, 4 dark cyan bags, 1 drab white bag.
pale gray bags contain 3 bright crimson bags, 1 bright yellow bag.
plaid maroon bags contain 1 pale coral bag.
dim violet bags contain 5 wavy blue bags, 4 shiny coral bags, 4 pale black bags, 3 posh yellow bags.
faded white bags contain 2 pale plum bags.
mirrored orange bags contain 3 striped maroon bags, 1 dull bronze bag, 2 pale aqua bags.
wavy black bags contain 1 faded violet bag, 1 plaid purple bag, 5 dim coral bags, 2 mirrored plum bags.
dim silver bags contain 2 muted brown bags, 1 posh plum bag.
dark tomato bags contain 5 dull magenta bags, 2 faded aqua bags, 3 vibrant lime bags, 3 drab silver bags.
dotted silver bags contain 2 dotted red bags.
muted yellow bags contain 4 drab purple bags, 3 striped olive bags.
shiny lavender bags contain 5 plaid purple bags, 2 vibrant crimson bags, 4 dull olive bags.
clear tomato bags contain 4 shiny bronze bags.
dark turquoise bags contain 5 dim brown bags.
drab coral bags contain 3 dull orange bags.
dotted beige bags contain 1 wavy lavender bag, 5 dull orange bags, 4 dull lime bags, 3 dull blue bags.
dark teal bags contain 3 muted indigo bags, 5 dark gold bags, 3 dull green bags, 2 plaid tan bags.
light lavender bags contain 2 dim maroon bags, 4 muted tomato bags, 4 dull plum bags.
dim lavender bags contain 4 light magenta bags, 4 shiny fuchsia bags, 5 wavy chartreuse bags, 1 faded chartreuse bag.
vibrant brown bags contain 4 dull bronze bags, 1 clear green bag, 3 posh orange bags, 3 mirrored silver bags.
pale tan bags contain 3 bright tomato bags, 5 pale maroon bags, 3 wavy teal bags.
muted black bags contain 1 light lime bag.
posh lime bags contain 3 bright crimson bags, 4 posh beige bags, 1 bright cyan bag.
light aqua bags contain 4 posh olive bags, 4 light tan bags, 4 wavy tan bags, 2 dotted blue bags.
dotted tan bags contain 3 dark yellow bags, 4 posh silver bags, 3 dark beige bags.
shiny black bags contain no other bags.
mirrored fuchsia bags contain 1 bright purple bag, 3 posh lime bags, 5 dim black bags.
dim purple bags contain 3 muted indigo bags.
wavy white bags contain 2 pale olive bags.
bright magenta bags contain 5 dull aqua bags, 3 dull green bags, 2 shiny turquoise bags.
drab white bags contain 1 plaid yellow bag, 2 shiny green bags, 5 posh beige bags, 5 plaid tan bags.
mirrored crimson bags contain 2 shiny teal bags.
pale white bags contain 2 muted indigo bags.
shiny bronze bags contain 5 mirrored crimson bags, 3 pale brown bags, 4 clear chartreuse bags.
clear blue bags contain 1 drab chartreuse bag, 4 striped turquoise bags, 1 plaid aqua bag, 5 pale aqua bags.
shiny brown bags contain 1 dotted bronze bag, 4 shiny fuchsia bags, 4 drab fuchsia bags, 4 dark lavender bags.
dotted teal bags contain 4 dull olive bags, 4 clear black bags.
posh gray bags contain 1 wavy salmon bag.
dark white bags contain 4 pale lime bags, 4 plaid beige bags, 5 dull fuchsia bags, 1 pale yellow bag.
dull cyan bags contain 4 muted lavender bags, 2 plaid tan bags, 2 dim blue bags.
dark olive bags contain 2 dim indigo bags, 1 dark brown bag, 5 dark yellow bags, 4 dull gray bags.
vibrant tan bags contain 4 bright tan bags, 1 pale tomato bag.
drab beige bags contain 4 muted magenta bags, 2 bright plum bags, 3 pale beige bags.
clear magenta bags contain 2 dull indigo bags, 5 vibrant green bags.
posh yellow bags contain no other bags.
vibrant magenta bags contain 2 dark green bags.
posh blue bags contain 2 muted chartreuse bags, 3 vibrant coral bags.
striped violet bags contain 1 dotted orange bag, 5 striped red bags, 5 dull lime bags.
dark yellow bags contain 2 shiny turquoise bags.
bright turquoise bags contain 2 bright yellow bags, 2 posh violet bags, 4 bright plum bags.
pale silver bags contain 4 vibrant bronze bags, 4 faded purple bags, 1 shiny green bag, 1 plaid lavender bag.
muted white bags contain 3 mirrored olive bags.
light green bags contain 5 shiny plum bags, 4 pale lavender bags, 5 faded gold bags.
drab lime bags contain 2 plaid beige bags.
pale chartreuse bags contain 4 dotted brown bags, 1 dark black bag, 3 mirrored plum bags, 1 faded black bag.
mirrored violet bags contain 5 plaid beige bags.
vibrant white bags contain 5 muted tomato bags.
faded violet bags contain 1 dull brown bag, 1 mirrored brown bag.
drab bronze bags contain 3 dim indigo bags, 1 dim magenta bag, 1 bright gray bag.
posh lavender bags contain 1 wavy black bag, 2 wavy magenta bags.
vibrant aqua bags contain 2 light lavender bags, 1 faded crimson bag, 4 dark green bags, 3 mirrored white bags.
bright green bags contain 1 dark gray bag.
shiny turquoise bags contain 2 bright yellow bags.
pale aqua bags contain 3 shiny gold bags, 2 drab silver bags, 3 drab salmon bags.
faded silver bags contain 3 striped teal bags, 5 mirrored indigo bags.
shiny indigo bags contain 3 wavy olive bags, 3 dim blue bags, 2 faded gold bags, 4 clear fuchsia bags.
vibrant yellow bags contain 5 dark chartreuse bags, 3 dull indigo bags, 1 bright tan bag.
shiny aqua bags contain 3 striped olive bags, 2 shiny teal bags, 5 pale teal bags.
bright maroon bags contain 2 striped lime bags.
dark purple bags contain 5 dotted brown bags, 1 bright olive bag, 3 drab orange bags, 1 mirrored olive bag.
drab lavender bags contain 5 wavy beige bags, 4 bright gold bags, 3 wavy cyan bags.
dull white bags contain 4 posh teal bags, 4 mirrored turquoise bags, 1 muted purple bag.
clear silver bags contain 1 dull lime bag, 2 dim plum bags, 5 wavy maroon bags, 3 striped cyan bags.
bright lavender bags contain 1 muted tomato bag.
pale crimson bags contain 1 striped tan bag, 4 dotted red bags, 3 dotted fuchsia bags, 5 dull brown bags.
posh maroon bags contain 3 dark magenta bags, 3 pale gray bags.
vibrant lime bags contain 1 dull maroon bag, 4 light purple bags, 5 dull orange bags, 1 dull purple bag.
drab turquoise bags contain no other bags.
dark bronze bags contain 2 light tomato bags, 1 vibrant gold bag, 3 posh yellow bags, 5 light lavender bags.
muted cyan bags contain 3 dotted black bags, 1 faded violet bag, 5 bright violet bags.
dim tomato bags contain 3 drab chartreuse bags, 1 striped coral bag, 3 pale tan bags.
light teal bags contain 3 pale gray bags.
muted olive bags contain 1 dull green bag, 3 bright violet bags, 1 dark yellow bag.
mirrored tan bags contain 3 dark salmon bags, 1 vibrant red bag, 1 dull gray bag.
light turquoise bags contain 5 posh red bags, 1 dim chartreuse bag, 3 vibrant beige bags, 2 mirrored lavender bags.
dark brown bags contain 2 clear fuchsia bags, 2 dull bronze bags, 3 striped aqua bags, 1 shiny gold bag.
striped orange bags contain 3 shiny gold bags, 1 bright crimson bag, 3 vibrant gold bags.
bright bronze bags contain 3 faded cyan bags, 3 dim blue bags, 2 faded gold bags, 2 bright tan bags.
dotted orange bags contain 4 dim magenta bags, 1 faded white bag, 1 muted tan bag.
dull red bags contain 3 posh yellow bags, 3 pale plum bags.
clear orange bags contain 5 wavy chartreuse bags, 5 dull aqua bags, 1 faded bronze bag, 5 faded chartreuse bags.
dark blue bags contain 2 striped lime bags.
wavy brown bags contain 4 shiny green bags.
wavy purple bags contain 2 dim maroon bags, 1 vibrant coral bag, 4 posh teal bags, 2 dark teal bags.
muted tan bags contain 5 bright violet bags, 4 dotted red bags, 5 dull beige bags, 1 dim gray bag.
posh crimson bags contain 3 dark bronze bags, 3 pale salmon bags, 2 muted beige bags, 2 bright aqua bags.
pale blue bags contain 3 wavy black bags, 5 dotted brown bags, 5 drab chartreuse bags, 5 bright gray bags.
striped plum bags contain 4 light turquoise bags.
posh green bags contain 5 dull cyan bags, 4 muted tan bags, 1 faded silver bag, 3 vibrant black bags.
mirrored yellow bags contain 5 shiny fuchsia bags, 3 bright tan bags, 4 drab fuchsia bags.
striped brown bags contain 4 wavy turquoise bags.
dim bronze bags contain 4 faded black bags, 1 striped lavender bag.
plaid cyan bags contain 5 posh lime bags, 2 wavy white bags, 3 posh crimson bags.
plaid brown bags contain 2 dark bronze bags.
vibrant blue bags contain 4 wavy orange bags, 4 drab silver bags, 2 muted tan bags.
dull violet bags contain 5 posh black bags, 4 shiny tomato bags.
striped magenta bags contain 3 faded white bags, 1 vibrant bronze bag.
striped coral bags contain 1 dull blue bag, 5 faded lime bags, 4 vibrant coral bags.
plaid lime bags contain 1 shiny black bag, 3 mirrored coral bags.
dim indigo bags contain 4 dim coral bags, 3 wavy indigo bags, 3 vibrant red bags, 3 wavy orange bags.
pale violet bags contain 2 drab purple bags.
striped turquoise bags contain 4 drab teal bags, 5 striped bronze bags.
striped indigo bags contain 3 striped purple bags, 4 shiny beige bags.
light coral bags contain 3 bright coral bags, 5 pale teal bags, 5 dull red bags, 3 mirrored tomato bags.
drab plum bags contain 4 dull white bags, 3 clear crimson bags, 2 wavy lavender bags.
dim maroon bags contain no other bags.
shiny crimson bags contain 2 dull indigo bags, 1 bright cyan bag.
dull aqua bags contain 3 dark yellow bags.
drab violet bags contain 4 striped aqua bags.
shiny gold bags contain 2 pale brown bags, 2 dotted chartreuse bags, 1 vibrant gold bag, 4 dull magenta bags.
plaid indigo bags contain 4 striped purple bags, 3 clear brown bags.
muted gold bags contain 1 shiny cyan bag.
dotted lime bags contain 3 dotted olive bags, 4 light tan bags, 4 bright bronze bags, 3 vibrant plum bags.
vibrant red bags contain 2 wavy red bags, 4 dotted chartreuse bags, 5 dim coral bags.
faded lime bags contain 5 drab turquoise bags, 4 pale lavender bags.
striped black bags contain 2 light white bags, 4 posh lime bags, 3 posh chartreuse bags.
posh brown bags contain 1 muted white bag, 2 striped teal bags, 1 faded chartreuse bag.
dotted aqua bags contain 5 dull orange bags.
clear yellow bags contain 5 pale fuchsia bags, 2 posh cyan bags.
faded crimson bags contain 4 vibrant black bags.
dull tan bags contain 1 plaid crimson bag.
dark chartreuse bags contain 1 mirrored turquoise bag.
clear purple bags contain 1 dotted magenta bag, 2 striped lavender bags.
pale indigo bags contain 2 drab tan bags, 2 shiny indigo bags.
plaid coral bags contain 2 dark olive bags, 1 pale aqua bag, 5 shiny bronze bags.
faded purple bags contain 4 dull aqua bags, 2 drab turquoise bags, 5 wavy crimson bags, 2 faded turquoise bags.
pale gold bags contain 3 plaid blue bags, 3 muted plum bags, 4 wavy tomato bags, 3 clear lavender bags.
plaid plum bags contain 1 plaid lavender bag, 3 wavy purple bags.
vibrant violet bags contain 1 posh teal bag, 4 dull gray bags, 5 dim aqua bags, 5 striped yellow bags.
light maroon bags contain 5 clear blue bags.
clear olive bags contain 4 posh yellow bags, 4 dotted olive bags, 2 faded chartreuse bags.
dotted indigo bags contain 1 muted crimson bag, 5 wavy purple bags, 3 dotted crimson bags, 4 dotted chartreuse bags.
muted coral bags contain 3 faded teal bags, 3 pale crimson bags, 1 posh fuchsia bag.
striped olive bags contain 1 plaid chartreuse bag, 1 dull indigo bag, 1 shiny salmon bag, 1 shiny crimson bag.
dull maroon bags contain 4 dull bronze bags, 1 mirrored brown bag, 5 wavy white bags, 5 clear teal bags.
bright aqua bags contain 2 light bronze bags, 3 bright violet bags, 5 posh fuchsia bags.
drab fuchsia bags contain 1 pale black bag.
plaid orange bags contain 2 muted orange bags.
faded aqua bags contain 4 striped orange bags, 3 striped aqua bags.
dull coral bags contain 2 dull aqua bags, 5 dim white bags, 1 striped tan bag.
muted blue bags contain 1 mirrored turquoise bag, 4 bright lavender bags, 4 dull magenta bags.
striped cyan bags contain 1 shiny tan bag, 5 dark cyan bags, 2 pale chartreuse bags, 3 muted gold bags.
muted violet bags contain 4 muted bronze bags, 5 mirrored tan bags, 5 bright yellow bags.
dull blue bags contain 3 striped fuchsia bags, 2 plaid aqua bags, 1 mirrored lime bag.
bright olive bags contain 1 pale turquoise bag, 2 vibrant gray bags, 5 posh chartreuse bags, 4 shiny lime bags.
drab red bags contain 1 light lime bag, 1 mirrored black bag, 4 pale purple bags, 2 drab purple bags.
mirrored brown bags contain 3 dull bronze bags, 3 bright lavender bags.
dotted brown bags contain 1 plaid turquoise bag, 1 dull plum bag.
shiny gray bags contain 2 pale magenta bags.
posh fuchsia bags contain 1 clear gold bag, 5 muted tomato bags, 4 muted white bags, 1 dark brown bag.
shiny green bags contain 5 posh teal bags, 2 dark brown bags, 1 faded turquoise bag.
shiny teal bags contain 1 bright coral bag, 2 plaid aqua bags, 2 vibrant yellow bags.
pale lime bags contain 5 shiny chartreuse bags, 2 striped gold bags.
bright tan bags contain 5 drab turquoise bags, 2 dotted plum bags.
wavy maroon bags contain 3 striped yellow bags, 2 plaid red bags, 4 pale teal bags.
light gray bags contain 3 wavy teal bags, 1 clear brown bag, 5 striped tan bags, 3 muted magenta bags.
dark tan bags contain 5 posh purple bags.
light lime bags contain 4 light brown bags, 3 faded violet bags, 5 bright plum bags.
bright gold bags contain 1 shiny crimson bag, 4 wavy red bags.
drab crimson bags contain 2 dotted purple bags, 4 plaid purple bags, 4 vibrant black bags.
pale maroon bags contain 1 dim gray bag.
wavy chartreuse bags contain 3 bright lavender bags.
plaid gold bags contain 5 posh lavender bags.
plaid yellow bags contain 3 shiny black bags.
dim red bags contain 2 muted indigo bags, 1 shiny aqua bag, 1 dim gray bag.
faded turquoise bags contain 2 shiny coral bags.
mirrored purple bags contain 3 muted beige bags, 4 faded blue bags.
dark black bags contain 4 bright coral bags.
dull indigo bags contain no other bags.
dark lime bags contain 1 shiny chartreuse bag, 5 drab plum bags, 2 dim yellow bags.
dim yellow bags contain 4 dotted plum bags, 2 dim coral bags, 3 dotted red bags.
shiny silver bags contain 5 drab tan bags, 1 bright cyan bag.
posh bronze bags contain 5 dotted salmon bags, 1 light aqua bag.
dull tomato bags contain 1 clear red bag, 1 dull green bag, 1 dark blue bag, 1 striped maroon bag.
bright fuchsia bags contain 1 dark cyan bag.
striped red bags contain 5 mirrored lime bags, 4 dull magenta bags, 2 vibrant yellow bags.
striped green bags contain 2 dim white bags.
wavy fuchsia bags contain 3 muted fuchsia bags, 3 drab blue bags.
posh violet bags contain 2 plaid red bags, 3 faded turquoise bags, 1 faded white bag.
vibrant maroon bags contain 1 clear fuchsia bag, 5 wavy purple bags, 2 dull gold bags.
dim olive bags contain 2 mirrored yellow bags, 2 muted magenta bags, 3 drab coral bags.
muted turquoise bags contain 1 drab white bag, 4 vibrant crimson bags, 5 dark teal bags, 1 drab bronze bag.
light white bags contain 5 vibrant gold bags, 2 faded silver bags, 1 bright beige bag.
dark beige bags contain 4 shiny white bags, 3 wavy chartreuse bags, 1 dotted red bag, 5 plaid crimson bags.
wavy gold bags contain 3 posh teal bags.
muted bronze bags contain 2 dull bronze bags, 2 muted black bags.
wavy lime bags contain 3 dim violet bags, 5 posh violet bags, 1 light white bag, 2 posh olive bags.
clear gray bags contain 2 pale purple bags.
vibrant black bags contain 5 plaid black bags.
dotted olive bags contain 4 dotted fuchsia bags.
drab gray bags contain 3 shiny chartreuse bags, 1 dim yellow bag.
shiny tomato bags contain 1 bright coral bag, 4 mirrored coral bags.
dotted coral bags contain 2 plaid cyan bags, 3 light crimson bags, 1 light white bag, 3 drab aqua bags.
drab blue bags contain 3 bright coral bags, 3 dim coral bags.
mirrored blue bags contain 5 dotted brown bags, 4 clear chartreuse bags, 2 bright gold bags, 3 striped lavender bags.
muted aqua bags contain 2 bright turquoise bags.
bright yellow bags contain no other bags.
)";

struct Inner {
  std::string kind;
  int num{};
};

struct Line {
  std::string outer;
  std::vector<Inner> inner;
};

std::istream &operator>>(std::istream &is, Line &line) {
  line.inner.clear();

  std::string l;
  std::getline(is, l);

  auto bags = l.find(" bag");

  if (bags == std::string::npos)
    return is;

  line.outer = l.substr(0, bags);

  while (true) {
    auto const number =
        std::find_if(std::next(l.begin(), bags), l.end(),
                     [](auto c) { return '0' <= c && c <= '9'; });

    if (number == l.end())
      return is;

    bags = l.find(" bag", std::distance(l.begin(), number));

    if (bags == std::string::npos)
      return is;

    line.inner.push_back(
        {std::string(number + 2, std::next(l.begin(), bags)), *number - '0'});
  }

  return is;
}

std::set<std::string>
count_containers(std::map<std::string, std::set<std::string>> const &map,
                 std::set<std::string> outermost, std::string const &to_find) {
  outermost.insert(to_find);

  if (auto const it = map.find(to_find); it != map.cend())
    for (auto &item : it->second)
      outermost = count_containers(map, std::move(outermost), item);

  return outermost;
}

int count_contents(std::map<std::string, std::vector<Inner>> const &map,
                   std::string const &to_find) {
  if (auto const it = map.find(to_find); it != map.cend())
    return std::accumulate(
        it->second.cbegin(), it->second.cend(), 0,
        [&](auto acc, auto const &item) {
          return acc + item.num * (1 + count_contents(map, item.kind));
        });

  return 0;
}

} // namespace

TEST_CASE("day7") {
  std::istringstream is{input};
  std::vector const lines(std::istream_iterator<Line>{is},
                          std::istream_iterator<Line>{});

  auto constexpr root = "shiny gold";

  auto const a = [&] {
    std::map<std::string, std::set<std::string>> containers;

    for (auto const &line : lines)
      for (auto const &contents : line.inner)
        containers[contents.kind].insert(line.outer);

    return count_containers(containers, {}, root).size() - 1;
  }();

  auto const b = [&] {
    std::map<std::string, std::vector<Inner>> containers;

    for (auto const &[key, value] : lines)
      containers.emplace(key, std::vector(value.cbegin(), value.cend()));

    return count_contents(containers, root);
  }();
}