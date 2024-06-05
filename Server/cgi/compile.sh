clean="rm -rf srcs/cgi_env.cgi srcs/cgi_post.cgi srcs/cgi_chunk.cgi srcs/cgi_static.cgi srcs/cgi_delete.cgi"

function compile()
{
	g++ cgi_chunk.cpp -o srcs/cgi_chunk.cgi
	g++ cgi_static.cpp -o srcs/cgi_static.cgi
	g++ cgi_env.cpp -o srcs/cgi_env.cgi
	g++ cgi_post.cpp -o srcs/cgi_post.cgi
	g++ cgi_toupper.cpp -o srcs/cgi_toupper.cgi
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