import os

for filename in sorted(os.listdir('tests')):
	print '\n========================================================================'
	print '>>>>>>>>>>>>  %s\n' % (filename)
	os.system('./limbaj.out %s' % (os.path.join(os.getcwd(), 'tests', filename)))
	print '========================================================================\n\n'
