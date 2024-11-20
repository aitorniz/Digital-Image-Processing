# How do I clone my git repo inside my docker
Run the following:
'''
git clone 
'''

#How do I change one branch in my local directory
'''
git switch branch-name
'''

#How do I run my container mounting my local repo and at the same time my git repo
Run the following:
'''
docker run -it -v /home/aitor/projects/Digital-Image-Processing:/root/projects/Digital-Image-Processing dip_dip_et_coyote /bin/bash
'''
