:: We are running on windows / mac
:: docker desktop makes everything root as per:
:: https://github.com/docker/for-mac/issues/6734
:: thus no user shenanigans necessary
docker run ^
	%= Run interactively =% ^
	-it ^
	%= Optional options =% ^
	%2 ^
	%= Mount entire repository to /vc to compile from =% ^
	--mount type=bind,source=%cd%,destination=/vc ^
	%= Run vc image generated by setup.sh =% ^
	vc ^
	%= Run argument script =% ^
	%1
