INPUT = open( "C:\\Users\\id181\\Documents\\ESP8266HotCold\\source\\html\\proxSensor.html", "r" )
OUTPUT = open( "C:\\Users\\id181\\Documents\\ESP8266HotCold\\source\\html\\proxSensor.txt", "w" )

start = 1
end   = 76
	
if __name__ == "__main__":
	idx = 1
	OUTPUT.write( "\"" )
	lines = INPUT.readlines()
	for line in lines:
		if ( idx >= start and idx <= end ) or False:
			s_line = line.strip()
			f_line = s_line.replace( "\"", "\\" + "\"" );
			OUTPUT.write( f_line )
		idx += 1
	OUTPUT.write( "\";" );
		