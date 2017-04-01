<?php
namespace app\index\controller;
use think\Controller;
class Lst extends Controller
{
    public function major()
    {
        $majorres=\think\Db::table('tju_major')->select();
        $this->assign('majorres',$majorres);
        return $this->fetch('major_list');
    }
    
    public function school()
    {
        $collegeres=\think\Db::table('tju_college')->paginate(5);
        $this->assign('collegeres',$collegeres);
        return $this->fetch('school_list');
    }
    
    public function academy()
    {
        $id=input('id');
        $academyres=\think\Db::table('tju_academy')->where('college_id',$id)->select();
        $this->assign('academyres',$academyres);
        return $this->fetch('academy_list');
    }
    
    public function teacher()
    {
        \think\Session::delete('academy_id');
        \think\Session::delete('major_id');
        $teacherres=\think\Db::table('tju_teacherinfo')->paginate(5);
        $this->assign('teacherres',$teacherres);
        return $this->fetch('teacher_list');
    }
    
    public function teacherbyacade()
    {
        $id=input('id');
        \think\Session::set('academy_id',$id);
        \think\Session::delete('major_id');
        $teacherres=\think\Db::table('tju_teacherinfo')->where('academy_id',$id)->paginate(5);
        $this->assign('teacherres',$teacherres);
        return $this->fetch('teacher_list');
    }
    
    public function teacherbymajor()
    {
        $id=input('id');
        \think\Session::set('major_id',$id);
        \think\Session::delete('academy_id');
        $teacherres=\think\Db::table('tju_teacherinfo')->where('major_id',$id)->paginate(5);
        $this->assign('teacherres',$teacherres);
        return $this->fetch('teacher_list');
    }  
    
    public function searchteacher()
    {
        $name=input('search');
        if ($name != null){
            $academy_id=\think\Session::get('academy_id');
            $major_id=\think\Session::get('major_id');
            if ($academy_id != null){
                $where=array(
                    "truename"=>$name,
                    "academy_id"=>$academy_id,
                );
                $teacherres=\think\Db::table('tju_teacherinfo')->where($where)->paginate(5);
                $this->assign('teacherres',$teacherres);
                return $this->fetch('teacher_list');
            }
            else if ($major_id != null){
                $where=array(
                    "truename"=>$name,
                    "major_id"=>$major_id,
                );
                $teacherres=\think\Db::table('tju_teacherinfo')->where($where)->paginate(5);
                $this->assign('teacherres',$teacherres);
                return $this->fetch('teacher_list');
            }
            else{
                $teacherres=\think\Db::table('tju_teacherinfo')->where('truename',$name)->paginate(5);
                $this->assign('teacherres',$teacherres);
                return $this->fetch('teacher_list');
            }
        }        
        \think\Session::delete('academy_id');
        \think\Session::delete('major_id');
        $teacherres=\think\Db::table('tju_teacherinfo')->paginate(5);
        $this->assign('teacherres',$teacherres);
        return $this->fetch('teacher_list');
    }
    
    public function searchschool()
    {
        $name=input('search');
        if ($name != null){
            $collegeres=\think\Db::table('tju_college')->where('name_college',$name)->paginate(5);
            $this->assign('collegeres',$collegeres);
            return $this->fetch('school_list');
        }        
        $collegeres=\think\Db::table('tju_college')->paginate(5);
        $this->assign('collegeres',$collegeres);
        return $this->fetch('school_list');
    }
    
    public function searchmajor()
    {
        $name=input('search');
        if ($name != null){
            $majorres=\think\Db::table('tju_major')->where('name_major',$name)->select();
            $this->assign('majorres',$majorres);
            return $this->fetch('major_searchlist');
        }        
        $majorres=\think\Db::table('tju_major')->select();
        $this->assign('majorres',$majorres);
        return $this->fetch('major_list');
    }
    
}
