import { h, Component } from 'preact';
import { auth } from '../firebase/firebase';

import Guard from './guard/guard';
import Form from './form/form';

interface MyProps {}
interface MyState {
    currentUser: any;
}

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            this.setState({ currentUser });
            // Could add callback that verifies their auth status + sets permission
            // to false
        });
    }

    render() {
        const user = this.state.currentUser;
        return user ? <Form userPermission /> : <Guard />;
    }
}
