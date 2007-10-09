#!/bin/csh

set name=cvs_backup

if ( ! $?SOURCEFORGE_CVS_BACKUP ) then
  echo
  echo "${name}: the SOURCEFORGE_CVS_BACKUP environment variable must be set"
  echo
  exit
endif

if ( "$1" == "" || "$1" == "-h" || "$1" == "--help" ) then
  echo
  echo "${name}: back up the CVS repository of a sourceforge project"
  echo
  echo "usage:"
  echo "  ${name} PROJECT"
  echo
  echo "where:"
  echo "  PROJECT is a sourceforge project name"
  echo
  echo "e.g. add a line like the following using crontab:"
  echo
  echo "SOURCEFORGE_CVS_BACKUP = /pulsar/sourceforge
  echo "00 00 * * * $SOURCEFORGE_CVS_BACKUP/cvs_backup.csh PROJECT"
  echo
  exit
endif

set project=$1

cd $SOURCEFORGE_CVS_BACKUP

if ( ! -d $project ) mkdir $project

cd $project

echo "Running rsync ${project}.cvs.sourceforge.net ..."

rsync -av "rsync://${project}.cvs.sourceforge.net/cvsroot/${project}/*" .

set file=`date +%Y_%m_%d_cvs_backup.tar.gz`

echo "Creating $file ..."

tar zcf `date +%Y_%m_%d_cvs_backup.tar.gz` *

