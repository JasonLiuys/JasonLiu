<?php
namespace app\index\validate;

use think\Validate;

class Login extends Validate
{
    protected $rule = [
        'account'  =>  'require',
        'password' => 'require|confirm:dbpassword',
    ];
    
    protected $message  =   [
        'account.require' => '账号不能为空',
        'password.require' => '密码不能为空',
        'password.confirm' => '用户名或密码错误',
    ];
}