<?php
namespace app\index\controller;
use think\Controller;
class Register extends Controller
{   
    public function register()
    {
        if(request()->isPost()){
            $time = time();
            $data=[
                'account'=>input('account'),
                'nickname'=>input('nickname'),
                'password'=>input('password'),
                'registtime'=>$time,
                'lock'=>0,
            ];
            $valdata=[
                'account'=>input('account'),
                'nickname'=>input('nickname'),
                'password'=>input('password'),
                'repassword'=>input('repassword'),
                'registtime'=>$time,
                'lock'=>0,
            ];
            $validate = \think\Loader::validate('register');
            if($validate->check($valdata)){
                $db=\think\Db::name('tju_users')->insert($data);
                if ($db){
                    $nickname = input('nickname');
                    $account = input('account');
                    $uid =\think\Db::table('tju_users')->where('account',$account)->value('id');
                    \think\Session::set('account',$account);
                    \think\Session::set('nickname',$nickname);
                    \think\Session::set('uid',$uid);
                    return $this->success('注册成功！','Index\index\homepage');
                }
                else{
                    return $this->error('数据库插入失败！');
                }
            }
            else{
                return $this->error($validate->getError());
            }
            return;
        }
        return $this->fetch('register');
    }
}
 