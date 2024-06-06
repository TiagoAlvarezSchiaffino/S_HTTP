clean="rm -rf srcs/cgi_lower.cgi srcs/multiple_workers"

function compile()
{
	g++ cgi_lower.cpp -o srcs/cgi_lower.cgi
	g++ multiple_workers.cpp -o multiple_workers
}

if [ "$1" == "clean" ]
then
	$clean
elif [ "$1" == "re" ]
then
	$clean
	compile
else
	compile
fi