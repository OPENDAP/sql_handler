#/bin/gawk

# 07/Aug/2010 carlo cancellieri ccancellieri@hotmail.com

BEGIN{
#das flag
	das=0;
	if (DAS==1 || DAS=="das"){
		d1=1;
	}
	else {
		d1=0;
	}
#dds flag
	dds=0;
	if (DDS==1 || DDS=="dds"){
		d2=1;
	}
	else {
		d2=0;
	}
#data flag
	data=0; 
	if (DATA==1 || DATA=="data"){
		d3=1;
	}
	else {
		d3=0;
	}
	
# simple or complete flag
	complete=0;
	simple=0;
	if (COMPLETE=="complete"){
		c=1;
		s=0;
	}
	else if (COMPLETE=="simple"){
		c=0;
		s=1;
	}
	else {
		c=0;
		s=0;
	}
# substitution
	toupper($CLASS);
	tolower($TYPE);
	
# All the rows flag
	rest=1;
}

/#DAS#/	{
	if (rest==0)	{ rest=1; das=0;} 
	else	{das=d1; dds=0;	data=0; rest=0;}
#	print "DAS: "das,dds,data,rest "COMPLETE:"complete,simple
	next;
}
/#DDS#/	{
	if (rest==0)	{ rest=1; dds=0;}
	else	{das=0; dds=d2; data=0; rest=0;}
#	print "DDS: "das,dds,data,rest "COMPLETE:"complete,simple
	next;
}
/#DATA#/{
	if (rest==0)	{ rest=1; data=0;}
	else	{das=0; dds=0; data=d3; rest=0;}
#	print "DATA: "das,dds,data,rest "COMPLETE:"complete,simple
	next;
}

/#COMPLETE#/{
	if (complete==1){complete=0;}
	else {complete=1;}
#	print "COMPLETE: "complete,c,rest
	next;
}
/#SIMPLE#/{
	if (simple==1)	{simple=0;}
	else {simple=1;}
#	print "SIMPLE: "simple,s,rest
	next;
}

//{	
	if (rest==1) {
		if (complete==1){
			#we are in the complete section
			if (c==1){
				#complete is what you want
				gsub(/OPENDAP_CLASS/,CLASS,$0);
				gsub(/OPENDAP_TYPE/,TYPE,$0);
				print $0
			}
			#else simple is what you want
		}
		else if (simple==1){
			#we are in the simple section
			if (s==1){
				#simple is what you want
				gsub(/OPENDAP_CLASS/,CLASS,$0);
				gsub(/OPENDAP_TYPE/,TYPE,$0);
				print $0
			}
			#else complete is what you want
		}
		else
		{
			#we are in the rest of the file
			gsub(/OPENDAP_CLASS/,CLASS,$0);
			gsub(/OPENDAP_TYPE/,TYPE,$0);
			print $0
		}
	}
	else if(das==1 || dds==1 || data==1) {
		if (complete==1){
			#we are in the complete section
			if (c==1){
				#complete is what you want
				gsub(/OPENDAP_CLASS/,CLASS,$0);
				gsub(/OPENDAP_TYPE/,TYPE,$0);
				print $0
			}
			#else simple is what you want
		}
		else if (simple==1){
			#we are in the simple section
			if (s==1){
				#simple is what you want
				gsub(/OPENDAP_CLASS/,CLASS,$0);
				gsub(/OPENDAP_TYPE/,TYPE,$0);
				print $0
			}
			#else complete is what you want
		}
		else
		{
			#we are in the rest of a (das==1 || dds==1 || data==1)
			gsub(/OPENDAP_CLASS/,CLASS,$0);
			gsub(/OPENDAP_TYPE/,TYPE,$0);
			print $0
		}
	}
}