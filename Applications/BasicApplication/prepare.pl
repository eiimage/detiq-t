#!/usr/bin/perl

`qmake -project`;
open PRO, "BasicApplication.pro";
open NPRO, ">NBasicApplication.pro";

$wr = 0;

while($ligne = <PRO>)
{
  if($w)
  {
    print NPRO $ligne;
  }

  if($ligne =~ /TEMPLATE/)
  {
    print NPRO "TEMPLATE = app\nLIBS += -lqwt -limagein -lpng -ljpeg -lQtInterface\n";
    $w = 1;
  }
}

close PRO;
close NPRO;

`mv NBasicApplication.pro BasicApplication.pro`;