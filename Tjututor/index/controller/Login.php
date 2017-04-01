<?php
namespace app\index\controller;
use think\Controller;
class Login extends Controller
{   
    public function login()
    {
        if(request()->isPost()){
            $time = time();
            $account = input('account');
            $data=[
                'account'=>input('account'),
                'password'=>input('password'),
            ];
            $db =\think\Db::table('tju_users')->where('account',$account)->value('password');
            $valdata=[
                'account'=>input('account'),
                'password'=>input('password'),
                'dbpassword'=>$db,
            ];
            $validate = \think\Loader::validate('login');
            if($validate->check($valdata)){
                $nickname =\think\Db::table('tju_users')->where('account',$account)->value('nickname');
                $uid =\think\Db::table('tju_users')->where('account',$account)->value('id');
                \think\Session::set('account',$account);
                \think\Session::set('nickname',$nickname);
                \think\Session::set('uid',$uid);
                return $this->success('登陆成功！','Index\index\homepage');
            }
            else{
                return $this->error($validate->getError());
            }
            return;
        }
        return $this->fetch('login');
    }
}
 