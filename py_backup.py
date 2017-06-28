import shutil, os, sys

def get_files(FROM_DIR, TO_DIR):
	for dirpath, subdirs, files in os.walk(FROM_DIR):
	    for x in files:
	    	back = (dirpath + '/' + x).replace(FROM_DIR, TO_DIR)
	    	back_dir = dirpath.replace(FROM_DIR, TO_DIR)
	    	if not os.path.exists(back_dir):
	    		os.makedirs(back_dir)
	    	shutil.copyfile(dirpath + '/' + x, (dirpath + '/' + x).replace(FROM_DIR, TO_DIR))
			


if __name__ == '__main__':
	if len(sys.argv) == 3:
		FROM_DIR = sys.argv[1]
		TO_DIR = sys.argv[2]
		get_files(FROM_DIR, TO_DIR)
	elif len(sys.argv) == 1:
		FROM_DIR = '/Users/cory/Development/Python'
		TO_DIR = '/Users/cory/Development/Backup'
		get_files(FROM_DIR, TO_DIR)
