$(document).ready(function () {
    var $signBtn = $("#signBtn"),
        $phoneInput = $("#phoneInput"),
        $pswInput = $("#pswInput"),
        $btnGroup = $("#btnGroup"),
        $voiceBtn = $("#voiceBtn"),
        $newContactBtn = $("#newContactBtn"),
        $newGroupBtn = $("#newGroupBtn"),
        $progress = $("#progress"),
        $signContainer = $("#signContainer"),
        $groupsPageUl = $("#groupsPageUl"),
        $contactsPageUl = $("#contactsPageUl"),
        $formPage = $("#formPage"),
        $formPageContainer = $("#formPageContainer"),
        account = {
            phone: "",
            contacts: {},
            groups: {},
            isNewAccount: false,
            operation: "",
            currentContact: ""
        },
        color = ["teal", "red", "blue", "amber"];


    // 去除字符串中的空格
    function removeSpace(str) {
        return str.replace(/\s/g, "");
    }

    // 处理登录事件
    function signIn(data) {
        console.log(data);
        if (data === "new account") {
            console.log("new account");
            account.isNewAccount = true;
            $btnGroup.fadeIn(1000);
            $signContainer.fadeOut(1000);
            setTimeout(function () {
                $signContainer.remove();
            }, 1000);
        } else if (data === "error password") {
            Materialize.toast("错误的密码或手机号, 请重新输入", 3000)
            $progress.addClass("determinate");
            $signBtn.addClass("pulse");
        } else if (data === "") {
            Materialize.toast("发生错误, 试试刷新浏览器或者再次提交", 3000)
            $progress.addClass("determinate");
            $signBtn.addClass("pulse");
        } else {
            var obj = JSON.parse(data);
            if (obj.contacts) {
                account.contacts = obj.contacts;
            }
            if (obj.groups) {
                account.groups = obj.groups;
            }
            account.isNewAccount = false;
            initUl();
            $btnGroup.fadeIn(1000);
            $signContainer.fadeOut(1000);
            setTimeout(function () {
                $signContainer.remove();
            }, 1000);
            console.log(account);
        }
    }

    function initUl() {
        var contactsHTML = "";
        var groupsHTML = "";
        for (var name in account.contacts) {
            var phone = account.contacts[name];
            contactsHTML += genContact(name, phone);
        }
        $(contactsHTML).appendTo($contactsPageUl);
        for (var groupName in account.groups) {
            groupsHTML += genGroup(groupName);
        }
        $(groupsHTML).appendTo($groupsPageUl);
        for (var groupName in account.groups) {
            console.log(`init have group ${groupName}`);
            contactsHTML = "";   
            var $group = $(`group-id-${groupName}`);     
            for (var name in account.groups[groupName]) {
                console.log(`init group ${groupName} have ${name}`)
                var phone = account.groups[groupName][name];
                var color = $group.data("color");
                contactsHTML += genContact(name, phone, "", groupName, color)
            }
            $(contactsHTML).appendTo(`#group-ul-id-${groupName}`);
        }
        addAllListener();
    }

    // 初始化页面
    function initPage() {
        $pageTabs = $('#pageTabs');

        // 将页面填充满屏
        $("._pages").height($(window).height() - $pageTabs.height());
        console.log($("._pages"));
        $pageTabs.tabs();

        $('select').material_select();

        $("#newContactBtn").click(function () {
            account.operation = "create contact";
            genForm();            
            $formPage.fadeIn();
        });

        $("#newGroupBtn").click(function () {
            account.operation = "create group";
            genForm();
            $formPage.fadeIn();
        });

        $formPage.click(function () {
            $formPage.fadeOut();
        });

        $formPageContainer.click(function (event) {
            event.stopPropagation();
        })
    }

    // 得到随机的颜色
    function getRandomColor() {
        var randomNum = Math.floor((Math.random()) * color.length);
        return color[randomNum];
    }

    function genContact(name, phone, className, groupName, groupColor) {
        var temp;

        // 创建在group中的contact
        if (groupName) {
            temp = `<li id="group-${groupName}-contact-${name}" 
                            class="${className}">
                        <div class="collapsible-header">
                            <i class="material-icons ${groupColor}-text">
                                    account_circle
                            </i>${name}
                        </div>
                        <div class="collapsible-body">
                            <div class="collection">
                                <a href="tel:${phone}" class="collection-item 
                                        blue-text">
                                    <i class="material-icons">call</i>打电话
                                </a>
                                <a href="sms:${phone}" class="collection-item 
                                        amber-text ">
                                    <i class="material-icons">message</i>
                                            发短信
                                </a>
                                <a href="#!" class="del-from-group 
                                        collection-item red-text" 
                                        data-group="${groupName}" 
                                        data-name="${name}">
                                    <i class="material-icons">delete</i>
                                            从群组<strong>${groupName}</strong>中删除
                                </a>
                            </div>
                        </div>
                    </li>`
        } else {
            var color = getRandomColor();
            temp = `<li id="contact-id-${name}" class="${className}">
                        <div class="collapsible-header">
                            <i class="material-icons ${color}-text">
                                    account_circle</i>${name}
                        </div>
                        <div class="collapsible-body">
                            <div class="collection">
                                <a href="tel:${phone}" class="collection-item 
                                        blue-text">
                                    <i class="material-icons">call</i>打电话
                                </a>
                                <a href="sms:${phone}" class="collection-item 
                                        amber-text ">
                                    <i class="material-icons">message</i>发短信
                                </a>
                                <a href="#!" class="add-to-group collection-item 
                                        teal-text" data-name="${name}">
                                    <i class="material-icons">group_add</i>
                                            加入到群组
                                </a>
                                <a href="#!" class="del-from-contact 
                                        collection-item red-text" 
                                        data-name="${name}">
                                    <i class="material-icons">delete</i>删除
                                </a>
                            </div>
                        </div>
                    </li>`
        }

        return temp;
    }

    // 生成群组的html
    function genGroup(groupName, className) {
        var color = getRandomColor();
        var temp = `<li id="group-id-${groupName}" data-color="${color}" 
                            class="${className}">
                        <div class="collapsible-header">
                            <i class="material-icons ${color}-text">group
                            </i>${groupName}
                            <span data-group="${groupName}" class=
                                    "del-group white-text badge red">delete
                            </span>
                        </div>
                        <div class="collapsible-body">
                            <ul id="group-ul-id-${groupName}" class="collapsible  
                                    popout" data-collapsible="accordion">
                            </ul>
                        </div>
                    </li>`;

        return temp;
    }

    // 创建group, 如果已经存在group则创建失败, 返回false, 否则向服务器发送请求创建
    function createGroup(groupName) {

        groupName = removeSpace(groupName);

        // 如果已经存在group则创建失败, 返回false
        if (account.groups.hasOwnProperty(groupName)) {
            return false;
        }

        $.post("../message", `set ${account.phone} groups ${groupName}\n`,
                function (data) {
            if (data === "ok") {
                account.groups[groupName] = {};
                var groupHTML = genGroup(groupName, "display-none");
                var $group = $(groupHTML);
                $group.appendTo($groupsPageUl);
                $group.fadeIn();
                addAllListener();
            };
        });

        return true;
    }

    function createContact(name, phone) {
        name = removeSpace(name);

        // 如果已经存在contact则创建失败
        if (account.contacts.hasOwnProperty(name)) {
            return false;
        }

        $.post("../message", `set ${account.phone} contacts ${name} ${phone}\n`,
                function (data) {
            if (data === "ok") {
                account.contacts[name] = phone;
                var contactHTML = genContact(name, phone, "display-none");
                var $contact = $(contactHTML);
                $contact.appendTo($contactsPageUl);
                $contact.fadeIn();
                addAllListener();
            }
        });

        return true;
    }

    function addContactToGroup(name, groupName) {
        name = removeSpace(name);
        groupName = removeSpace(groupName);
        var phone = account.contacts[name];
        $.post("../message", 
                `set ${account.phone} groups ${groupName} ${name} ${phone}\n`, function (data) {
            if (data === "ok") {
                var $group = $(`group-id-${groupName}`);
                var contactHTML = genContact(name, phone, "display-none",
                        groupName, $group.data("color"));
                var $contact = $(contactHTML);
                $contact.appendTo(`#group-ul-id-${groupName}`);
                $contact.fadeIn();
                addAllListener();
            }
        });

        return true;
    }

    // 删除群组, 如果不存在, 则返回false
    function deleteGroup(groupName) {
        groupName = removeSpace(groupName);
        if (!account.groups.hasOwnProperty(groupName)) {
            return false;
        }
        console.log(`i will delete group ${groupName}`);
        $.post("../message", 
                `del ${account.phone} groups ${groupName}\n`, 
                function (data) {
            if (data === "ok") {
                account.groups[groupName] = null;
                delete account.groups[groupName];
                $group = $(`#group-id-${groupName}`);
                $group.fadeOut();
                setTimeout(function () {
                    $group.remove();
                }, 1500);
            }
        });

        return true;
    }

    function deleteContact(name) {
        console.log(`i will delete ${name}`);
        console.log(typeof name);
        name = removeSpace(name);
        if (!account.contacts.hasOwnProperty(name)) {
            return false;
        }

        $.post("../message", `del ${account.phone} contacts ${name}\n`, 
                function (data) {
            if (data === "ok") {
                delete account.contacts[name];
                $contact = $(`#contact-id-${name}`);
                $contact.fadeOut();
                setTimeout(function () {
                    $contact.remove();
                }, 1500);
            }
        });

        return true;
    }

    function deleteContactFromGroup(name, groupName) {
        name = removeSpace(name);
        groupName = removeSpace(groupName);
        console.log(`i will delete ${name} from ${groupName}`);
        // if (!account[groupName] || !account[groupName].hasOwnProperty(name)) {
        //     return false;
        // }

        $.post("../message", 
                `del ${account.phone} groups ${groupName} ${name}\n`,
                function (data) {
            if (data === "ok") {
                delete account.groups[groupName][name];
                var $contact = $(`#group-${groupName}-contact-${name}`);
                $contact.fadeOut();
                setTimeout(function () {
                    $contact.remove();
                }, 1500)
            }
        });

        return true;
    }

    // 在外部创建handle函数, 避免使用addAllListener重复添加
    function handleDeleteContact() {
        var $contact = $(this),
            name = String($contact.data("name"));
        
        console.log(name);
        if (deleteContact(name)) {
            Materialize.toast("删除成功", 3000);
        } else {
            Materialize.toast("删除失败", 3000);
        }
    }

    function handleDeleteContactFromGroup() {
        var $contact = $(this),
            name = String($contact.data("name")),
            groupName = String($contact.data("group"));
        console.log(name);
        console.log(groupName);
        if (deleteContactFromGroup(name, groupName)) {
            Materialize.toast("删除成功", 3000);
        } else {
            Materialize.toast("删除失败", 3000);
        }
    }

    function handleDeleteGroup() {

        var $group = $(this),
            groupName = String($group.data("group"));
        console.log(groupName);
        if (deleteGroup(groupName)) {
            Materialize.toast("删除成功", 3000);
        } else {
            Materialize.toast("删除失败", 3000);
        }
    }

    function handleAddContactToGroup() {
        var name = $(this).data("name");
        console.log(name);
        account.currentContact = String(name);
        account.operation = "add contact to group";
        genForm();        
        $formPage.fadeIn();
    }

    // 为所有需要动态添加监听的元素加入监听
    function addAllListener() {

        // 使列表可折叠
        $(".collapsible").collapsible();

        // 监听删除联系人按钮的点击
        var delFromContactTags = document.querySelectorAll(".del-from-contact");
        for (var i = 0; i < delFromContactTags.length; i++) {
            delFromContactTags[i].addEventListener("click", handleDeleteContact)
        }
        // $(".del-from-contact").bind("click", handleDeleteContact);

        // 监听从群组删除联系人
        var delContactFromGroupTags = document.querySelectorAll(".del-from-group");
        for (var i = 0; i < delContactFromGroupTags.length; i++) {
            delContactFromGroupTags[i].addEventListener("click",
                    handleDeleteContactFromGroup);
        }
        // $(".del-from-group").bind("click", handleDeleteContactFromGroup);

        // 监听删除群组
        var delGroupTags  = document.querySelectorAll(".del-group");
        for (var i = 0; i < delGroupTags.length; i++) {
            delGroupTags[i].addEventListener("click", handleDeleteGroup);
        }
        // $(".del-group").bind("click", handleDeleteGroup);

        // 监听加入群组按钮的点击
        var addToGroupTags = document.querySelectorAll(".add-to-group");
        for (var i = 0; i < addToGroupTags.length; i++) {
            addToGroupTags[i].addEventListener("click",
                    handleAddContactToGroup);
        }
        // $(".add-to-group").bind("click", handleAddContactToGroup);
    }

    function genForm() {
        var temp;
        console.log(account.operation);
        if (account.operation === "add contact to group") {
            temp = `<div class="input-field col s8 offset-s2">
                        <select id="selectGroupInput" class="teal-text">
                            <option value="" disabled selected>选择要加入的群组
                            </option>`;
            for (var groupName in account.groups) {

                // 如果该联系人不在那个群组中的话, 添加到选项中
                if (!(account.currentContact in account.groups[groupName])) {
                    temp += `<option value="${groupName}">${groupName}
                             </option>`;
                }
            }
            temp += `</select></div>
                        <button id="confirmBtn" class="col s6 m2 offset-s3 offset-m5 btn waves-effect waves-light" type="submit" name="action">提交
                            <i class="material-icons right">send</i>
                        </button>`;
        } else if (account.operation === "create contact") {
            temp = `<div class="input-field col s8 m4 offset-s2 offset-m4">
                        <i class="material-icons prefix">account_circle</i>
                        <input id="nameInput" type="text" class="validate">
                        <label for="nameInput">姓名</label>
                    </div>
                    <div class="input-field col s8 m4 offset-s2 offset-m4">
                        <i class="material-icons prefix">phone</i>
                        <input id="telInput" type="tel" 
                                class="validate">
                        <label for="telInput">电话号码</label>
                    </div>
                    <button id="confirmBtn" class="col s6 m2 offset-s3 offset-m5 btn waves-effect waves-light" type="submit" name="action">提交
                        <i class="material-icons right">send</i>
                    </button>`;
        } else if (account.operation === "create group") {
            temp = `<div class="input-field col s8 m4 offset-s2 offset-m4">
                        <i class="material-icons prefix">account_circle</i>
                        <input id="groupNameInput" type="tel" class="validate">
                        <label for="groupNameInput">新建群组名</label>
                    </div>
                    <button id="confirmBtn" class="col s6 m2 offset-s3 offset-m5 btn waves-effect waves-light" type="submit" name="action">提交
                        <i class="material-icons right">send</i>
                    </button>`;
        } else {
            Materialize.toast("出现了一些错误, 刷新页面试试", 3000);
        }

        // 设置弹出内容, 绑定点击事件
        $formPageContainer.empty();
        $formPageContainer.html(temp);
        $('select').material_select();
        $("#confirmBtn").click(getFormPageData);
    }

    function getFormPageData() {
        if (account.operation === "add contact to group") {
            var groupName = String(removeSpace($("#selectGroupInput").val()));
            console.log(groupName);
            console.log(account.currentContact);
            if (groupName) {
                addContactToGroup(account.currentContact, groupName);
                $formPage.fadeOut();
            } else {
                Materialize.toast("请选择要加入的群组名", 3000);
            }
        } else if (account.operation === "create contact") {
            var name = removeSpace($("#nameInput").val()),
                phone = removeSpace($("#telInput").val());
            if (phone && name) {
                createContact(name, phone);
                $formPage.fadeOut();
            } else {
                Materialize.toast("请输入姓名和电话", 3000);
            }

        } else if (account.operation === "create group") {
            var groupName = removeSpace($("#groupNameInput").val());
            if (groupName) {
                createGroup(groupName);
                $formPage.fadeOut();
            } else {
                Materialize.toast("请输入新建群组名", 3000);
            }
        } else {
            return;
        }
    }


    initPage();

    // 监听登录
    $signBtn.click(function () {
        $progress.removeClass("determinate");
        $progress.addClass("indeterminate");
        $signBtn.removeClass("pulse");
        account.phone = $phoneInput.val();
        var phoneNum = parseInt(account.phone);

        password = removeSpace($pswInput.val());
        // console.log(phoneNum);

        // 如果不是数字或者密码为空, 阻止登录
        if ((account.phone.length != 11) || isNaN(phoneNum) ||
            (phoneNum < 10000000000) || (password === "")) {
            Materialize.toast("请输入密码和11位的电话号码", 3000);
            $progress.removeClass("indeterminate");
            $progress.addClass("determinate");
            $signBtn.addClass("pulse");

            return;
        }

        // 向服务器发送账号密码, 成功登录则初始化页面
        $.post("../message", `init ${account.phone} ${password}\n`,
            signIn);
    });

});