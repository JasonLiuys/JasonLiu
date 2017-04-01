<?php
namespace app\index\validate;

use think\Validate;

class Register extends Validate
{
    protected $rule = [
        'account'  =>  'require|max:25',
        'nickname' => 'require|max:25',
        'password' => 'require|min:5|confirm:repassword',
    ];
    
    protected $message  =   [
        'account.require' => '账号不能为空',
        'account.max' => '账号长度不能超过25位',
        'nickname.require' => '昵称不能为空',
        'nickname.max' => '昵称长度不能超过25位',
        'password.require' => '密码不能为空',
        'password.confirm' => '两次密码输入不一致',
        'password.min' => '密码长度必须为六位以上'
    ];
}