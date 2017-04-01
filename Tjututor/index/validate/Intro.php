<?php
namespace app\index\validate;

use think\Validate;

class Intro extends Validate
{
    protected $rule = [
        'content'  =>  'require',
        'tju_users_id' => 'require',
    ];
    
    protected $message  =   [
        'content.require' => '评论不能为空',
        'tju_users_id.require' => '请先登录',
    ];
}