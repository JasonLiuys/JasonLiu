<?php
namespace app\index\controller;
use think\Controller;
class Index extends Controller
{
    public function index()
    {
        \think\Session::clear(); //重新登陆前销毁之前session
        return $this->fetch('login');
    }
    
    public function homepage()
    {
        return $this->fetch('index');
    }
}
