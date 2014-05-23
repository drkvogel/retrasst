## storeman log in

Hello both 
 
I have modified the log-in screen to select the appropriate database system for the new setup 
 
    Live = vnode_vlab_64::ldb* (the default for the release version) 
    Test = vnode_vlab_64::test_ldb* 
    Mirror = vnode_labdev_64::ldb* 
    Dev = vnode_labdev_64::t_ldb* (the default for the debug version) 
    Db Test = vnode_labdev_64::test_ldb* 
 
Please make sure you have your vnodes set up correctly. Don't use "mirror" if you're not sure. 
 
Jonathan: can you use the database connection from the log-in screen in your code? I'm happy to change my code to make that easier 
-- 
Nick 