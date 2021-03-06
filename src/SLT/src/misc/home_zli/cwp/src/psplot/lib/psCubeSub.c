/* Copyright (c) Colorado School of Mines, 1990.
/* All rights reserved.                       */

#include "cwp.h"
#include "psplot.h"

void psAxes3(
	float x,float y,float size1,float size2,float size3,float angle,
	float x1Beg, float x1End, float p1Beg, float p1End, 
	float d1Num, float f1Num, int n1Tic, int grid1, char *label1,
	float x2Beg, float x2End, float p2Beg, float p2End,
	float d2Num, float f2Num, int n2Tic, int grid2, char *label2,
	float x3Beg, float x3End, float p3Beg, float p3End,
	float d3Num, float f3Num, int n3Tic, int grid3, char *label3,
	char *labelFont, float labelSize,
	char *title, char *titleFont, float titleSize,
	float x1cut, float x2cut, float x3cut)
/*
FUNCTION:  plot axes, tics, labels and titles of a cube via PostScript

PARAMETERS:
x		 x coordinate of lower left corner of box
y		 y coordinate of lower left corner of box
size1		 size of 1st dimension of the input cube
size2		 size of 2nd dimension of the input cube
size3		 size of 3rd dimension of the input cube
angle		 projection angle of the cube	 
x1Beg  		 axis value at beginning of axis 1
x1End		 axis value at end of axis 1
p1Beg  		 pad value at beginning of axis 1
p1End		 pad value at end of axis 1
d1Num		 numbered tic increment for axis 1 (0.0 for automatic)
f1Num		 first numbered tic for axis 1
n1Tic		 number of tics for axis 1
grid1		 grid code for axis 1:  NONE, DOT, DASH, or SOLID
label1		 label for axis 1
x2Beg  		 axis value at beginning of axis 2
x2End		 axis value at end of axis 2
p2Beg  		 pad value at beginning of axis 2
p2End		 pad value at end of axis 2
d2Num		 numbered tic increment for axis 2 (0.0 for automatic)
f2Num		 first numbered tic for axis 2
n2Tic		 number of tics for axis 2
grid2		 grid code for axis 2:  NONE, DOT, DASH, or SOLID
label2		 label for axis 2
x3Beg  		 axis value at beginning of axis 3
x3End		 axis value at end of axis 3
p3Beg  		 pad value at beginning of axis 3
p3End		 pad value at end of axis 3
d3Num		 numbered tic increment for axis 3 (0.0 for automatic)
f3Num		 first numbered tic for axis 3
n3Tic		 number of tics for axis 3
grid3		 grid code for axis 3:  NONE, DOT, DASH, or SOLID
label3		 label for axis 3
labelFont	 name of font to use for axes labels
labelSize	 size of font to use for axes labels
title		 axes box title
titleFont	 name of font to use for title
titleSize	 size of font to use for title
x1cut            slice section at axis 1
x2cut            slice section at axis 2
x3cut            slice section at axis 3

Author: 	 Zhiming Li & Dave Hale,     6/90    CSM 
*/
{
	float xxx,yyy,xx,yy;
	int n1num,n2num,n3num,ntic,ndash,grided;
	float xa,ya,ticsize,dnum,fnum,dtic,amin,amax,azero,
		width,height,base,scale,anum,atic,
		ticb,numb,labelb,dash[2],
		labelCH,labelCW,labelCA,labelCD,
		titleCH,titleCW,titleCA,titleCD;
	char str[256];

	/* determine font dimensions */
	labelCH = fontheight(labelFont,labelSize);
	labelCW = fontwidth(labelFont,labelSize);
	labelCA = fontascender(labelFont,labelSize);
	labelCD = fontdescender(labelFont,labelSize);
	titleCH = fontheight(titleFont,titleSize);
	titleCW = fontwidth(titleFont,titleSize);
	titleCA = fontascender(titleFont,titleSize);
	titleCD = fontdescender(titleFont,titleSize);

	/* determine numbered tic intervals */
	if (d1Num==0.0) {
		n1num = size1/(4*labelCW);
		scaxis(x1Beg,x1End,&n1num,&d1Num,&f1Num);
	}
	if (d2Num==0.0) {
		n2num = size2/(4*labelCW);
		scaxis(x2Beg,x2End,&n2num,&d2Num,&f2Num);
	}
	if (d3Num==0.0) {
		n3num = size3/(4*labelCW);
		scaxis(x3Beg,x3End,&n3num,&d3Num,&f3Num);
	}


	/* save graphic status */
	gsave();

	/* translate coordinate system, so that origin is at x,y */
	translate(x,y);

	/* erase two triagles */
	gsave();
	newpath();
	yyy = size1; xxx=0.;
	moveto(xxx,yyy);
	yyy = yyy + size3*sin(angle);
	lineto(xxx,yyy);
	xxx = xxx + size3*cos(angle);
	lineto(xxx,yyy);
	closepath();
	setgray(1.);
	fill();
	grestore();

	gsave();
	newpath();
	yyy = 0.; xxx=size2;
	moveto(xxx,yyy);
	xxx = xxx + size3*cos(angle);
	lineto(xxx,yyy);
	yyy = yyy + size3*sin(angle);
	lineto(xxx,yyy);
	closepath();
	setgray(1.0);
	fill();
	grestore();
	

	/* set font and character size */
	setfont(labelFont,labelSize);

	/* determine tic size */
	ticsize = 0.3*labelCH;

	height = size1 + size3*sin(angle); 
	width = size2 + size3*cos(angle); 

	/* draw axis 1 */
	translate(0.,size1);
	rotate(-90.0);

	amin = (x1Beg<x1End)?x1Beg:x1End;
	amax = (x1Beg>x1End)?x1Beg:x1End;
	azero = 0.0001*(amax-amin);
	dnum = d1Num;  fnum = f1Num;  ntic = n1Tic;
	scale = size1/(x1End+p1End-x1Beg-p1Beg);
	base = -scale*(x1Beg+p1Beg);
	ticb = -ticsize;
	numb = ticb-labelCA;
	labelb = numb-labelCW-0.5*labelCA;
	for (anum=fnum; anum<=amax; anum+=dnum) {
		if (anum<amin) continue;
		xa = base+scale*anum;
		moveto(xa,0.0);  lineto(xa,ticb);
		if (anum>-azero && anum<azero)
			sprintf(str,"%.4g",0.0);
		else
			sprintf(str,"%.4g",anum);
		xx = xa + 0.3*labelCH; 
		moveto(xx,numb);
		rotate(90.);
		justshow(-0.5,str);
		rotate(-90.);
	}
	stroke();
	dtic = dnum/ntic;
	for (atic=fnum-ntic*dtic-dtic; atic<=amax; atic+=dtic) {
		if (atic<amin) continue;
		xa = base+scale*atic;
		moveto(xa,0.0);  lineto(xa,ticb/2);
	}
	stroke();
	if (grid1==SOLID) {
		grided = 1;
		ndash = 0;
	} else if (grid1==DASH) {
		grided = 1;
		ndash = 1;  dash[0] = 10;
	} else if (grid1==DOT) {
		grided = 1;
		ndash = 2;  dash[0] = 1;  dash[1] = 5;
	} else
		grided = 0;
	if (grided) {
		for (anum=fnum; anum<=amax; anum+=dnum) {
			if (anum<amin) continue;
			xa = base+scale*anum;
			moveto(xa,0.0);  lineto(xa,size2);
			xa = xa - size3*sin(angle);
			ya = size2+size3*cos(angle);
			lineto(xa,ya);
		}
		setdash(dash,ndash,0.0);
		stroke();
		setdash(dash,0,0.0);
	}
	if (x1cut > x1Beg && x1cut < x1End ) {
		ndash = 0;
		xa = base+scale*x1cut;
		moveto(xa,0.0);  lineto(xa,size2);
		xa = xa - size3*sin(angle);
		ya = size2+size3*cos(angle);
		lineto(xa,ya);
		setdash(dash,ndash,0.0);
		stroke();
		setdash(dash,0,0.0);
	}
	moveto(size1/2.0,labelb);
	rotate(180.);
	justshow(-0.5,label1);
	rotate(-180.);

	rotate(90.0);
	translate(0.,-size1);


	/* draw axis 2 */
	xxx = width - size2; 
	translate(xxx,height);
	rotate(-90.);

	amin = (x2Beg<x2End)?x2Beg:x2End;
	amax = (x2Beg>x2End)?x2Beg:x2End;
	azero = 0.0001*(amax-amin);
	dnum = d2Num;  fnum = f2Num;  ntic = n2Tic;
	scale = size2/(x2End+p2End-x2Beg-p2Beg);
	base = -scale*(x2Beg+p2Beg);
	ticb = -ticsize;
	numb = ticb+labelCD;
	labelb = numb-labelCH;
	for (anum=fnum; anum<=amax; anum+=dnum) {
		if (anum<amin) continue;
		ya = base+scale*anum;
		moveto(0.0,ya);  lineto(ticb,ya);
		if (anum>-azero && anum<azero)
			sprintf(str,"%.4g",0.0);
		else
			sprintf(str,"%.4g",anum);
		moveto(numb,ya); 
		rotate(90.0); 
		justshow(-0.5,str);
		rotate(-90.0);
	}
	stroke();
	dtic = dnum/ntic;
	for (atic=fnum-ntic*dtic-dtic; atic<=amax; atic+=dtic) {
		if (atic<amin) continue;
		ya = base+scale*atic;
		moveto(0.0,ya);  lineto(ticb/2,ya);
	}
	stroke();
	if (grid2==SOLID) {
		grided = 1;
		ndash = 0;
	} else if (grid2==DASH) {
		grided = 1;
		ndash = 1;  dash[0] = 10;
	} else if (grid2==DOT) {
		grided = 1;
		ndash = 2;  dash[0] = 1;  dash[1] = 5;
	} else
		grided = 0;
	if (grided) {
		for (anum=fnum; anum<=amax; anum+=dnum) {
			if (anum<amin) continue;
			ya = base+scale*anum;
			moveto(0.0,ya);  
			ya = ya-size3*cos(angle);
			xa = size3*sin(angle);
			lineto(xa,ya);
			xa = xa+size1;
			lineto(xa,ya);
		}
		setdash(dash,ndash,0.0);
		stroke();
		setdash(dash,0,0.0);
	}
	if (x2cut > x2Beg && x2cut < x2End ) {
		ndash = 0;
		ya = base+scale*x2cut;
		moveto(0.0,ya);  
		ya = ya-size3*cos(angle);
		xa = size3*sin(angle);
		lineto(xa,ya);
		xa = xa+size1;
		lineto(xa,ya);
		setdash(dash,ndash,0.0);
		stroke();
		setdash(dash,0,0.0);
	}
	moveto(labelb,size2/2.0);
	rotate(90.0);
	justshow(-0.5,label2);

	xxx = width - size2; 
	translate(-xxx,-height);
	
	/* draw axis 3 */
	xxx = size1; 
	translate(0.,xxx);
	yyy = angle*180./3.141592654;
	rotate(yyy);

	amin = (x3Beg<x3End)?x3Beg:x3End;
	amax = (x3Beg>x3End)?x3Beg:x3End;
	azero = 0.0001*(amax-amin);
	dnum = d3Num;  fnum = f3Num;  ntic = n3Tic;
	scale = size3/(x3End+p3End-x3Beg-p3Beg);
	base = -scale*(x3Beg+p3Beg);
	ticb = ticsize;
	numb = ticb+labelCA;
	labelb = numb+labelCH;
	for (anum=fnum; anum<=amax; anum+=dnum) {
		if (anum<amin) continue;
		xa = base+scale*anum;
		xx = xa - ticb*cos(angle);
		yy = ticb*sin(angle); 
		moveto(xa,0.0);  lineto(xx,yy);
		if (anum>-azero && anum<azero)
			sprintf(str,"%.4g",0.0);
		else
			sprintf(str,"%.4g",anum);
		xx = xa - numb * cos(angle) - 0.5*labelCH;
		yy = numb * sin(angle);
		moveto(xx,yy);
		rotate(-yyy);
		justshow(-0.5,str);
		rotate(yyy);
	}
	stroke();
	dtic = dnum/ntic;
	for (atic=fnum-ntic*dtic-dtic; atic<=amax; atic+=dtic) {
		if (atic<amin) continue;
		xa = base+scale*atic;
		xx = xa - ticb/2.*cos(angle);
		yy = ticb/2. * sin(angle);
		moveto(xa,0.0);  lineto(xx,yy);
	}
	stroke();
	if (grid3==SOLID) {
		grided = 1;
		ndash = 0;
	} else if (grid3==DASH) {
		grided = 1;
		ndash = 1;  dash[0] = 10;
	} else if (grid3==DOT) {
		grided = 1;
		ndash = 2;  dash[0] = 1;  dash[1] = 5;
	} else
		grided = 0;
	if (grided) {
		for (anum=fnum; anum<=amax; anum+=dnum) {
			if (anum<amin) continue;
			xa = base+scale*anum;
			moveto(xa,0.0);  
			xa = xa + size2*cos(angle);
		        ya = -size2*sin(angle);
			lineto(xa,ya);
			xa = xa - size1*sin(angle);
			ya = ya - size1*cos(angle);
			lineto(xa,ya);
		}
		setdash(dash,ndash,0.0);
		stroke();
		setdash(dash,0,0.0);
	}
	if (x3cut > x3Beg && x3cut < x3End ) {
		ndash = 0;
		xa = base+scale*x3cut;
		moveto(xa,0.0);  
		xa = xa + size2*cos(angle);
	        ya = -size2*sin(angle);
		lineto(xa,ya);
		xa = xa - size1*sin(angle);
		ya = ya - size1*cos(angle);
		lineto(xa,ya);
		setdash(dash,ndash,0.0);
		stroke();
		setdash(dash,0,0.0);
	}
	xxx = size3/2.0;
	moveto(xxx,labelb);
	justshow(-0.5,label3);

	yyy = angle*180./3.141592654;
	rotate(-yyy);
	xxx = size1; 
	translate(0.,-xxx);

	/* draw title */
	setfont(titleFont,titleSize);
	xxx = size2/2.;
	moveto(xxx,-0.5*titleCH-titleCA);
	justshow(-0.5,title);

	/* draw axes box */
	moveto(0.0,0.0);
	yyy=0.; xxx = size2;
	lineto(xxx,yyy);
	yyy=size1;
	lineto(xxx,yyy);
	xxx=0.;
	lineto(xxx,yyy);
	yyy=0.;
	lineto(xxx,yyy);
	xxx=0.;yyy=size1;
	moveto(xxx,yyy);
	xxx=xxx+size3*cos(angle);
	yyy=yyy+size3*sin(angle);
	lineto(xxx,yyy);
	xxx = xxx + size2;
	lineto(xxx,yyy);
	xxx = size2; yyy=size1;
	lineto(xxx,yyy);
	xxx = size2; yyy=0.;
	moveto(xxx,yyy);
	xxx=xxx+size3*cos(angle);
	yyy=yyy+size3*sin(angle);
	lineto(xxx,yyy);
	yyy=yyy+size1;
	lineto(xxx,yyy);
	stroke();

	/* restore graphics state */
	grestore();
}

void cubesize(float hf, float ht, float wf, float ws, 
	      float *vsize, float *hsize, 
	      int n1f, int n1t, int n2f, int n2s, 
	      int *n1total, int *n2total) 
/* 
=== compute cube size 
   ---	input:
	hf		height of front panel (cm or inch)
	ht		height of top panel after projection (cm or inch)
	wf		width of fornt panel (cm or inch)
	ws		width of side panel after projection (cm or inch)
	n1f		number of vertical points of front panel 
	n1t		number of vertical points of top panel after projection
	n2f		number of horizontal points of front panel 
	n2s		number of horizontal points of side panel after 
			projection
   ---	output:
	vsize		vertical size of the cube (cm or inch) 		
	hsize		horizontal size of the cube (cm or inch)
	n1total		number of vertical grids of the cube	
	n2total		number of horizontal grids of the cube	

Author: 	 Zhiming Li,     6/90    CSM 
*/
{
	*vsize = hf + ht;
	*n1total = n1f + n1t - 1 ;
	*hsize = wf + ws; 
	*n2total = n2f + n2s - 1;
}

void wside(float wb, float *wa, float d2, float angle, int *n2a)
/* 
=== compute width of side panel  
   ---	input:
	wb 		width of side panel before projection  (cm or inch)
	d2		size of horizontal grid (cm or inch)
	angle		angle of projection
   ---	output:
	wa 		width of side panel after projection  (cm or inch)
	n2a		number of horizontal grids of the projected side panel 	

Author: 	 Zhiming Li,     6/90    CSM 
*/
{
	float temp;
	*wa = wb * cos(angle);
	temp = *wa/d2 + 1.5;
	*n2a = temp;
	*wa = (*n2a-1.)*d2;
}

void sidestretch(float *sideb, float *sidea, int n2b, int n2a, int n1)
/* 
=== horizontal stretch of side panel  
   ---	input:
	*sideb		side panel before stretch (n1*n2b)
	n2b		number of horizontal grids of side panel before stretch
	n2a		number of horizontal grids of side panel after stretch
	n1		number of vertical grids of side panel
   ---	output:
	*sidea		side panel after stretch (n1*n2a)

Author: 	 Zhiming Li,     6/90    CSM 
*/
{
	int ix, j, i, ja, jb, jbp1;
	float dx, x, tmp;
	float d2a,d2b;
	d2b = n2b; d2a = n2a;
	tmp = d2b/d2a;
	for (j=0;j<n2a;j++)
	{
	   x = j * tmp;
	   ix = x;
	   dx = x - ix;
	   ja = j*n1;
	   jb = ix*n1;
	   jbp1 = (ix+1)*n1;
	   if ( ix < n2b-1 ) 
      	   {
	      for (i=0;i<n1;i++)
	         sidea[ja+i] = sideb[jb+i]+dx*(sideb[jbp1+i]-sideb[jb+i]);
	   }
	   else if (ix == n2b-1)
	   {
	      for (i=0;i<n1;i++)
	         sidea[ja+i] = sideb[jb+i];
	   }
	   else
	   {
	      for (i=0;i<n1;i++)
	         sidea[ja+i] = 0.;
	   }
        }
}


void htop(float hb, float *ha, float d1, float angle, int *n1a)
/* 
=== compute height of top panel  
   ---	input:
	hb 		height of top panel before projection  (cm or inch)
	d1		size of vertical grid (cm or inch)
	angle		angle of projection
   ---	output:
	ha 		height of top panel after projection  (cm or inch)
	n1a		number of vertical grids of the projected top panel 	

Author: 	 Zhiming Li,     6/90    CSM 
*/
{
	float temp;
	*ha = hb * sin(angle);
	temp = *ha/d1 + 1.5;
	*n1a = temp;
	*ha = (*n1a-1.)*d1;
}

void topstretch(float *topb, float *topa, int n1b, int n1a, int n2)
/* 
=== vertical stretch of top panel  
   ---	input:
	*topb		top panel before stretch (n1b*n2)
	n1b		number of vertical grids of top panel before stretch
	n1a		number of vertical grids of top panel after stretch
	n2		number of horizontal grids of top panel
   ---	output:
	*topa		top panel after stretch (n1b*n2)

Author: 	 Zhiming Li,     6/90    CSM 
*/
{
	int ix, j, i, jb, ja ;
	float dx, x, d1a, d1b, tmp;
	d1a = n1a; d1b = n1b;
	tmp = d1b/d1a;
	for (j=0;j<n2;j++)
	{
	   jb = j*n1b;
	   ja = j*n1a; 
	   for (i=0;i<n1a;i++)
	   {
	      x = i * tmp;
	      ix = x;
	      dx = x - ix;
	      if ( ix < n1b-1 ) 
      	      {
	         topa[ja+i] = topb[jb+ix]+dx*(topb[jb+ix+1]-topb[jb+ix]);
	      }
	      else if (ix == n1b-1)
	      {
	         topa[ja+i] = topb[jb+ix];
	      }
	      else
	      {
	         topa[ja+i] = 0.;
	      }
	   }
        }
}


void cubemap(int n1front, int n2front, 
	     int n1side, int n2side,
             int n1top, int n2top, 
	     float d1cu, float d2cu,
	     float front[], float side[],
	     float top[], float cube[], float angle)
/* 
=== map three stretched panels of a cube to a plane  
   ---	input:
	n1front		number of samples of first dimension of front panel 
	n2front		number of samples of second dimension of front panel 
	n1side		number of samples of first dimension of side panel 
	n2side		number of samples of second dimension of side panel 
	n1top		number of samples of first dimension of top panel 
	n2top		number of samples of second dimension of top panel 
			(n1front=n1side; n2front=n2top; n2side=n1top)
	d1cu		sample plot interval of first dimension (cm or inch)
	d2cu		sample plot interval of second dimension (cm or inch)
	front[]		front panel 
	side[]		side panel (after stretch in sidestretch()) 
	top[]		top panel (after stretch in topstretch()) 
	angle		angle of projection
   ---	output:
	cube[] 		mapped plane view of the cube (n1s*n2s) 		
			where n1s and n2s are computed in cubesize()

Author: 	 Zhiming Li,     6/90    CSM 
*/
{ 
	int i,j,jside,itop;
	int n1s, n2s,ibeg,jbeg,iend,jend,idisp,jdisp;	
	float dx,dy,fbeg,disp;
	double x,tangle;
	float tmp;

	n1s = n1front + n1top - 1;
	n2s = n2front + n2side - 1;
	for (i=0;i<n1s*n2s;i++) cube[i] = 0.;
/* copy front panel */
	for (j=0;j<n2front;j++)
	   for (i=0;i<n1front;i++)
	      cube[j*n1s+(i+n1top-1)] = front[j*n1front+i];
/* map side panel */
	tmp = d2cu/d1cu; 
	x = angle;
	tangle = tan(x);
        for (j=1;j<n2side;j++)
	{
	   disp = j*tmp; disp = disp * tangle;
	   fbeg = n1top; fbeg = fbeg - (disp+0.5); ibeg = fbeg;
	   if (ibeg < 0 ) ibeg = 0; if (ibeg > n1s-1 ) ibeg = n1s-1;
           iend = ibeg+n1side;
	   if (iend < 0 ) iend = 0; if (iend > n1s-1 ) iend = n1s-1;
	   idisp=disp; 
	   dx = disp-idisp; 
	   jside = j+n2front-1;
	   for (i=ibeg;i<iend-1;i++)
	   {
	      cube[jside*n1s+i] = side[j*n1side+i-ibeg]
	          + dx*(side[j*n1side+i+1-ibeg]-side[j*n1side+i-ibeg]);
	   }
	   cube[jside*n1s+iend-1] = side[j*n1side+iend-1-ibeg];
	}
/* map top panel */
	tmp = d1cu/d2cu;
        for (i=1;i<n1top;i++)
	{
	   disp = i*tmp; disp = disp / tangle;
	   fbeg = disp+0.5; jbeg = fbeg;
	   if (jbeg < 0 ) jbeg = 0; if (jbeg >= n2s-1 ) jbeg = n2s-2;
           jend = jbeg+n2top-1;
	   if (jend < 0 ) jend = 0; if (jend >= n2s-1 ) iend = n2s-2;
	   jdisp=disp; 
	   dy = disp-jdisp; 
	   itop = n1top - i - 1;
	   for (j=jbeg;j<jend;j++)
	   {
	      cube[j*n1s+itop] = top[(j-jbeg)*n1top+i]
	          + dy*(top[(j-jbeg+1)*n1top+i]-top[(j-jbeg)*n1top+i]);
	   }
	}
}	


void cubeimage(int n1, int n2, int n3, float size1, float size2, float size3, 
	       float angle, float *front, float *side, float *top, 
	       float *cube)
/* 
=== map three panels of a cube to a plane  
   ---	input:
	n1		number of samples of first dimension 
	n2		number of samples of second dimension 
	n3		number of samples of third dimension 
	size1		plot size of first dimension (cm or inch)
	size2		plot size of second dimension (cm or inch)
	size3		size of third dimension (cm or inch)
	angle		angle of projection
	*front		front panel (n1*n2)
	*side		side panel (n1*n3)
	*top		top panel (n3*n2) 
   ---	output:
	*cube 		mapped plane view of the cube (n1s*n2s) 		
			where n1s and n2s are computed in cubesize()

Author: 	 Zhiming Li,     6/90    CSM 
*/
{
	int i,j;
	int n2a,n1a;
	float *sidea,*topa;
	float wa,ha;
	float d1, d2, tmp;

	tmp = n1-1; d1 = size1 / tmp;
	tmp = n2-1; d2 = size2 / tmp; 

/* side panel stretch */
	wside(size3, &wa, d2, angle, &n2a);
	sidea = (float*)malloc(n2a*n1*sizeof(float));
	sidestretch(side, sidea, n3, n2a, n1);
/* top panel stretch */ 
        htop(size3, &ha, d1, angle, &n1a);
	topa = (float*)malloc(n1a*n2*sizeof(float));
        topstretch(top, topa, n3, n1a, n2);

/* cube map */
	cubemap(n1, n2, n1, n2a, n1a, n2, d1, d2,
	        front, sidea, topa, cube, angle);
	free(sidea);
	free(topa);
}
