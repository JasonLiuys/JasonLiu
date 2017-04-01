<?php
namespace app\index\controller;
use think\Controller;
class Intro extends Controller
{
    public function teacher()
    {
        $id=input('id') or $id=\think\Session::get('tid',$id);
        $teacher=\think\Db::table('tju_teacherinfo')->where('id',$id)->select();
        $comment=\think\Db::table('tju_comment')->where('tid',$id)->paginate(5);
        $this->assign('teacher',$teacher);
        $this->assign('comment',$comment);
        \think\Session::set('tid',$id);
        return $this->fetch('teacher_intro');
    }

    public function evaluate()
    {
        $id=input('id');
        $teacher=\think\Db::table('tju_teacherinfo')->where('id',$id)->select();
        $this->assign('teacher',$teacher);
        if(request()->isPost()){
            $time = time();
            $tid = \think\Session::get('tid');
            $uid = \think\Session::get('uid');
            $data=[
                'score1'=>input('score1'),
                'score2'=>input('score2'),
                'score3'=>input('score3'),
                'content'=>input('text'),
                'tid'=>$tid,
                'tju_users_id'=>$uid,
                'time'=>$time,
            ];
            $validate = \think\Loader::validate('intro');
            if($validate->check($data)){
                $db=\think\Db::name('tju_comment')->insert($data);
                if ($db){
                    return $this->success('添加评论成功！','Index\intro\teacher');
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
        return $this->fetch('teacher_evaluate');
    }
}

